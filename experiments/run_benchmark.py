#!/usr/bin/env python3
"""Chạy thực nghiệm hàng loạt cho routecraft, có thể tiếp tục khi bị ngắt."""

import argparse
import csv
import datetime
import glob
import json
import os
import re
import shlex
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed

COLUMNS = [
    "instance", "set", "customers", "k", "config", "status", "total_distance",
    "num_routes", "bks", "gap_pct", "vehicles_minus_k", "construct_ms",
    "total_ms", "git_sha", "timestamp",
]
P_CORE_MASK = 0xFFF
MAX_JOBS = 6
DEFAULT_EXE = "build/Release/bin/Release/routecraft"
DEFAULT_INSTANCES = "data/benchmarks/cvrplib/*/*.vrp"
DEFAULT_BKS = "data/benchmarks/cvrplib/bks.csv"
DEFAULT_OUT = "runtime-output/experiments/results.csv"


def read_bks(path):
    """Trả về dict instance -> dòng bks.csv; rỗng nếu thiếu file."""
    if not path or not os.path.isfile(path):
        return {}
    with open(path, newline="", encoding="utf-8") as f:
        return {row["instance"]: row for row in csv.DictReader(f)}


def compute_gap(total, bks):
    if total is None or bks is None or bks == 0:
        return ""
    return round((total - bks) / bks * 100, 4)


def read_dimension(path):
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            for line in f:
                m = re.match(r"\s*DIMENSION\s*:\s*(\d+)", line)
                if m:
                    return int(m.group(1))
    except OSError:
        pass
    return None


def instance_info(path, bks):
    """Trả về (instance, set, customers, k, bks_value) cho một file .vrp."""
    name = os.path.splitext(os.path.basename(path))[0]
    set_name = os.path.basename(os.path.dirname(os.path.abspath(path)))
    row = bks.get(name)
    customers = k = bks_val = None
    if row:
        customers = int(row["customers"]) if row.get("customers") else None
        k = int(row["k"]) if row.get("k") else None
        bks_val = float(row["bks"]) if row.get("bks") else None
    if customers is None:
        dim = read_dimension(path)
        customers = dim - 1 if dim else None
    if k is None:
        m = re.search(r"-k(\d+)", name)
        k = int(m.group(1)) if m else None
    return name, set_name, customers, k, bks_val


def read_rows(path):
    if not os.path.isfile(path):
        return []
    with open(path, newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def effective_rows(rows):
    """Dòng sau cùng của mỗi khóa (set, instance, config) là dòng có hiệu lực."""
    eff = {}
    for r in rows:
        eff[(r["set"], r["instance"], r["config"])] = r
    return eff


def done_keys(rows):
    return {k for k, r in effective_rows(rows).items() if r["status"] == "ok"}


def git_sha():
    try:
        sha = subprocess.run(["git", "rev-parse", "--short", "HEAD"],
                             capture_output=True, text=True, check=True).stdout.strip()
        dirty = subprocess.run(["git", "status", "--porcelain"],
                               capture_output=True, text=True, check=True).stdout.strip()
        return sha + ("-dirty" if dirty else "")
    except (OSError, subprocess.CalledProcessError):
        return "unknown"


def pin_cpu():
    n = os.cpu_count() or 1
    if n < 12:
        print("Cảnh báo: máy có dưới 12 CPU logic, bỏ qua ghim CPU.", file=sys.stderr)
        return
    try:
        if os.name == "nt":
            import ctypes
            k32 = ctypes.windll.kernel32
            k32.GetCurrentProcess.restype = ctypes.c_void_p
            k32.SetProcessAffinityMask.argtypes = [ctypes.c_void_p, ctypes.c_size_t]
            if not k32.SetProcessAffinityMask(k32.GetCurrentProcess(), P_CORE_MASK):
                print("Cảnh báo: SetProcessAffinityMask thất bại.", file=sys.stderr)
        elif hasattr(os, "sched_setaffinity"):
            os.sched_setaffinity(0, set(range(12)))
        else:
            print("Cảnh báo: nền tảng không hỗ trợ ghim CPU.", file=sys.stderr)
    except Exception as e:  # noqa: BLE001
        print(f"Cảnh báo: không ghim được CPU: {e}", file=sys.stderr)


def run_solver(exe, path, config_args, timeout):
    """Chạy một lần; trả về dict gồm status và các trường kết quả."""
    cmd = [exe, "solve", "--input", path, "--format", "json"] + config_args
    try:
        p = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8",
                           errors="replace", timeout=timeout)
    except subprocess.TimeoutExpired:
        return {"status": "timeout"}
    except OSError as e:
        return {"status": "error", "message": str(e)}
    if p.returncode != 0:
        return {"status": "error", "message": p.stderr.strip()[:200]}
    try:
        data = json.loads(p.stdout)
        t = data.get("timings_ms", {})
        return {
            "status": "ok" if data.get("feasible") else "infeasible",
            "total_distance": data["total_distance"],
            "num_routes": data["num_routes"],
            "construct_ms": t.get("construct", ""),
            "total_ms": sum(v for v in t.values() if isinstance(v, (int, float))),
        }
    except (ValueError, KeyError, TypeError) as e:
        return {"status": "error", "message": f"JSON không hợp lệ: {e}"}


def make_row(info, config, result, sha, now=None):
    name, set_name, customers, k, bks_val = info
    total = result.get("total_distance")
    routes = result.get("num_routes")
    return {
        "instance": name, "set": set_name,
        "customers": "" if customers is None else customers,
        "k": "" if k is None else k,
        "config": config, "status": result["status"],
        "total_distance": "" if total is None else total,
        "num_routes": "" if routes is None else routes,
        "bks": "" if bks_val is None else bks_val,
        "gap_pct": compute_gap(total, bks_val),
        "vehicles_minus_k": "" if routes is None or k is None else routes - k,
        "construct_ms": result.get("construct_ms", ""),
        "total_ms": result.get("total_ms", ""),
        "git_sha": sha,
        "timestamp": now or datetime.datetime.now(datetime.timezone.utc).isoformat(timespec="seconds"),
    }


def run_all(paths, configs, bks, out, jobs, run_fn, sha):
    """Chạy các ca chưa có dòng ok; ghi từng dòng ngay. Trả về (ok, lỗi, bỏ qua)."""
    done = done_keys(read_rows(out))
    tasks = []
    skipped = 0
    for path in paths:
        info = instance_info(path, bks)
        for cname, cargs in configs:
            if (info[1], info[0], cname) in done:
                skipped += 1
            else:
                tasks.append((path, info, cname, cargs))
    ok = bad = 0
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    new_file = not os.path.isfile(out) or os.path.getsize(out) == 0
    with open(out, "a", newline="", encoding="utf-8") as f, \
            ThreadPoolExecutor(max_workers=jobs) as ex:
        w = csv.DictWriter(f, fieldnames=COLUMNS)
        if new_file:
            w.writeheader()
            f.flush()
        futs = {ex.submit(run_fn, path, cargs): (info, cname)
                for path, info, cname, cargs in tasks}
        for fut in as_completed(futs):
            info, cname = futs[fut]
            try:
                res = fut.result()
            except Exception as e:  # noqa: BLE001
                res = {"status": "error", "message": str(e)}
            w.writerow(make_row(info, cname, res, sha))
            f.flush()
            if res["status"] == "ok":
                ok += 1
            else:
                bad += 1
                print(f"{res['status']}: {info[1]}/{info[0]} [{cname}] {res.get('message', '')}",
                      file=sys.stderr)
    return ok, bad, skipped


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--exe", default=DEFAULT_EXE)
    ap.add_argument("--instances", action="append")
    ap.add_argument("--bks", default=DEFAULT_BKS)
    ap.add_argument("--config", action="append", default=[], metavar="NAME=ARGS")
    ap.add_argument("--out", default=DEFAULT_OUT)
    ap.add_argument("--jobs", type=int, default=MAX_JOBS)
    ap.add_argument("--timeout", type=float, default=60)
    ap.add_argument("--no-pin", action="store_true")
    a = ap.parse_args(argv)
    for s in (sys.stdout, sys.stderr):
        s.reconfigure(errors="replace")

    exe = a.exe
    if os.name == "nt" and not exe.lower().endswith(".exe"):
        exe += ".exe"
    exe = os.path.abspath(exe)
    if not os.path.isfile(exe):
        print(f"Không thấy exe: {exe}", file=sys.stderr)
        return 2
    configs = []
    for c in a.config:
        name, sep, args = c.partition("=")
        if not sep or not name:
            print(f"--config sai định dạng (cần NAME=ARGS): {c}", file=sys.stderr)
            return 2
        configs.append((name, shlex.split(args)))
    if not configs:
        print("Cần ít nhất một --config.", file=sys.stderr)
        return 2
    paths = sorted({p for pat in (a.instances or [DEFAULT_INSTANCES]) for p in glob.glob(pat)})
    if not paths:
        print("Không có instance nào khớp.", file=sys.stderr)
        return 2
    jobs = max(1, min(a.jobs, MAX_JOBS))
    if not a.no_pin:
        pin_cpu()

    bks = read_bks(a.bks)
    run_fn = lambda path, cargs: run_solver(exe, path, cargs, a.timeout)  # noqa: E731
    ok, bad, skipped = run_all(paths, configs, bks, a.out, jobs, run_fn, git_sha())
    print(f"Xong: ok={ok} lỗi={bad} bỏ qua={skipped}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
