#!/usr/bin/env python3
"""Tổng hợp kết quả thực nghiệm thành bảng Markdown."""

import argparse
import csv
import statistics
import sys


def load_effective(paths):
    eff = {}
    for p in paths:
        with open(p, newline="", encoding="utf-8") as f:
            for r in csv.DictReader(f):
                eff[(r["set"], r["instance"], r["config"])] = r
    return eff


def num(r, col):
    try:
        return float(r[col])
    except (KeyError, ValueError, TypeError):
        return None


def stats_row(rows):
    ok = [r for r in rows if r["status"] == "ok"]

    def col(c, f=statistics.mean):
        vals = [v for v in (num(r, c) for r in ok) if v is not None]
        return f"{f(vals):.2f}" if vals else "-"

    return [str(len(rows)), str(len(ok)), col("gap_pct"),
            col("gap_pct", statistics.median), col("num_routes"),
            col("vehicles_minus_k"), col("total_ms", statistics.median)]


def warnings(eff):
    out = []
    for r in eff.values():
        g, d = num(r, "gap_pct"), num(r, "vehicles_minus_k")
        if g is not None and d is not None and g < 0 and d <= 0:
            out.append(f"{r['set']}/{r['instance']} [{r['config']}]: gap={g} m-k={d:g}")
    return sorted(out)


def summarize(eff):
    head = ["set", "config", "instances", "ok", "gap TB (%)", "gap trung vị (%)",
            "số xe TB", "m-k TB", "total_ms trung vị"]
    lines = ["| " + " | ".join(head) + " |", "|" + "---|" * len(head)]
    groups = {}
    for (s, _, c), r in eff.items():
        groups.setdefault((s, c), []).append(r)
    for (s, c) in sorted(groups):
        lines.append("| " + " | ".join([s, c] + stats_row(groups[(s, c)])) + " |")
    by_cfg = {}
    for (_, _, c), r in eff.items():
        by_cfg.setdefault(c, []).append(r)
    for c in sorted(by_cfg):
        lines.append("| **tổng** | **" + c + "** | " + " | ".join(
            "**" + x + "**" for x in stats_row(by_cfg[c])) + " |")
    text = "\n".join(lines) + "\n"
    w = warnings(eff)
    if w:
        text += "\nCảnh báo: gap < 0 mà m-k <= 0 (nghi có lỗi):\n\n" + "\n".join("- " + x for x in w) + "\n"
    return text


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("csv", nargs="+")
    ap.add_argument("--out")
    a = ap.parse_args(argv)
    text = summarize(load_effective(a.csv))
    if a.out:
        with open(a.out, "w", encoding="utf-8") as f:
            f.write(text)
    else:
        sys.stdout.reconfigure(encoding="utf-8")
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
