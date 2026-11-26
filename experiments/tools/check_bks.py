#!/usr/bin/env python3
"""Kiem tra bo benchmark CVRPLIB trong data/benchmarks/cvrplib.

Voi moi dong trong bks.csv: doc .vrp va .sol, kiem tra moi khach xuat hien dung
mot lan, tai moi tuyen khong vuot CAPACITY, tinh lai chi phi theo EUC_2D lam
tron tung canh, so voi dong "Cost" trong .sol va cot bks. Thoat ma 1 neu sai.

Quy uoc danh so: trong .sol khach duoc danh so 1..n-1; depot la node 1 trong
.vrp, nen khach i trong .sol ung voi node i+1 trong .vrp.
"""
import argparse
import csv
import math
import re
import sys
from pathlib import Path

DEFAULT_ROOT = Path(__file__).resolve().parents[2] / "data" / "benchmarks" / "cvrplib"


def parse_vrp(path):
    header = {}
    coords = {}
    demand = {}
    depots = []
    section = None
    for raw in path.read_text(encoding="ascii").splitlines():
        line = raw.strip()
        if not line:
            continue
        if line == "EOF":
            break
        if line.endswith("_SECTION"):
            section = line
            continue
        if section is None:
            key, _, val = line.partition(":")
            header[key.strip()] = val.strip()
        elif section == "NODE_COORD_SECTION":
            i, x, y = line.split()[:3]
            coords[int(i)] = (int(float(x)), int(float(y)))
        elif section == "DEMAND_SECTION":
            i, d = line.split()[:2]
            demand[int(i)] = int(d)
        elif section == "DEPOT_SECTION":
            v = int(line.split()[0])
            if v != -1:
                depots.append(v)
    if header.get("TYPE") != "CVRP" or header.get("EDGE_WEIGHT_TYPE") != "EUC_2D":
        raise ValueError("TYPE/EDGE_WEIGHT_TYPE khong phai CVRP/EUC_2D")
    dim = int(header["DIMENSION"])
    if len(coords) != dim or len(demand) != dim:
        raise ValueError("so node khong khop DIMENSION")
    if depots != [1]:
        raise ValueError("depot phai la node 1, gap %r" % depots)
    return dim, int(header["CAPACITY"]), coords, demand


def parse_sol(path):
    routes = []
    cost = None
    for raw in path.read_text(encoding="ascii").splitlines():
        line = raw.strip()
        m = re.match(r"Route\s*#\d+\s*:(.*)$", line)
        if m:
            routes.append([int(t) for t in m.group(1).split()])
        elif line.startswith("Cost"):
            cost = int(line.split()[1])
    if cost is None:
        raise ValueError("thieu dong Cost")
    return routes, cost


def edge(coords, a, b):
    dx = coords[a][0] - coords[b][0]
    dy = coords[a][1] - coords[b][1]
    return int(math.sqrt(dx * dx + dy * dy) + 0.5)


def check_instance(root, row):
    name, sset = row["instance"], row["set"]
    base = root / sset / name
    dim, cap, coords, demand = parse_vrp(base.with_suffix(".vrp"))
    routes, sol_cost = parse_sol(base.with_suffix(".sol"))
    errors = []
    if dim - 1 != int(row["customers"]):
        errors.append("customers=%s != DIMENSION-1=%d" % (row["customers"], dim - 1))
    seen = {}
    total = 0
    for r, route in enumerate(routes, 1):
        nodes = [c + 1 for c in route]  # id trong .sol -> id node trong .vrp
        for c in route:
            if not 1 <= c <= dim - 1:
                errors.append("tuyen %d: khach %d ngoai khoang" % (r, c))
        nodes = [n for n in nodes if 2 <= n <= dim]
        for n in nodes:
            seen[n] = seen.get(n, 0) + 1
        load = sum(demand[n] for n in nodes)
        if load > cap:
            errors.append("tuyen %d: tai %d > Q=%d" % (r, load, cap))
        path = [1] + nodes + [1]
        total += sum(edge(coords, a, b) for a, b in zip(path, path[1:]))
    missing = [n for n in range(2, dim + 1) if seen.get(n, 0) == 0]
    dup = [n for n, c in seen.items() if c > 1]
    if missing:
        errors.append("thieu khach (node vrp): %s" % missing)
    if dup:
        errors.append("khach lap: %s" % dup)
    if total != sol_cost:
        errors.append("chi phi tinh lai %d != Cost trong .sol %d" % (total, sol_cost))
    if total != int(row["bks"]):
        errors.append("chi phi tinh lai %d != bks %s" % (total, row["bks"]))
    if len(routes) != int(row["k"]):
        # chi la thong tin: k chi de tham chieu
        pass
    return len(routes), total, errors


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--root", type=Path, default=DEFAULT_ROOT)
    args = ap.parse_args()
    root = args.root
    with open(root / "bks.csv", newline="", encoding="ascii") as f:
        rows = list(csv.DictReader(f))
    listed = {(r["set"], r["instance"]) for r in rows}
    bad = 0
    print("%-12s %3s %5s %6s %7s %8s  %s" % ("instance", "set", "cust", "routes", "k", "cost", "status"))
    for row in rows:
        try:
            nr, cost, errs = check_instance(root, row)
        except (OSError, ValueError) as e:
            nr, cost, errs = 0, 0, ["loi doc file: %s" % e]
        status = "OK" if not errs else "FAIL"
        print("%-12s %3s %5s %6d %7s %8d  %s" % (row["instance"], row["set"], row["customers"], nr, row["k"], cost, status))
        for e in errs:
            print("    - " + e)
        bad += bool(errs)
    on_disk = {(p.parent.name, p.stem) for p in root.glob("*/*.vrp")}
    for s, n in sorted(on_disk - listed):
        print("FAIL: %s/%s.vrp khong co trong bks.csv" % (s, n))
        bad += 1
    for s, n in sorted(listed - on_disk):
        print("FAIL: %s/%s co trong bks.csv nhung thieu .vrp" % (s, n))
        bad += 1
    print("\n%d instance, %d loi" % (len(rows), bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
