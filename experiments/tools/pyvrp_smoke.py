"""Smoke test: PyVRP solves a .vrp instance under the project's conventions.

Checks that PyVRP's cost equals an independent recomputation with TSPLIB
EUC_2D rounding per edge, and that the solution is a valid CVRP solution.
"""
import argparse
import csv
import math
import re
import sys
from pathlib import Path

import pyvrp
from pyvrp.stop import MaxRuntime

ROOT = Path(__file__).resolve().parents[2]


def parse_vrp(path):
    """Return (capacity, coords, demands); index 0 is the depot."""
    capacity, coords, demands, section = None, {}, {}, None
    for line in Path(path).read_text().splitlines():
        s = line.strip()
        if not s or s == "EOF":
            continue
        if s.endswith("SECTION"):
            section = s
            continue
        m = re.match(r"CAPACITY\s*:\s*(\d+)", s)
        if m:
            capacity = int(m.group(1))
        elif section == "NODE_COORD_SECTION":
            i, x, y = s.split()[:3]
            coords[int(i)] = (float(x), float(y))
        elif section == "DEMAND_SECTION":
            i, d = s.split()[:2]
            demands[int(i)] = int(d)
        elif section == "DEPOT_SECTION":
            section = None
    n = len(coords)
    ids = [i for i in range(1, n + 1)]
    return capacity, [coords[i] for i in ids], [demands[i] for i in ids]


def edge(a, b):
    return int(math.sqrt((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) + 0.5)


def route_cost(route, coords):
    path = [0] + route + [0]
    return sum(edge(coords[a], coords[b]) for a, b in zip(path, path[1:]))


def find_bks(name):
    with open(ROOT / "data/benchmarks/cvrplib/bks.csv", newline="") as f:
        for row in csv.DictReader(f):
            if row["instance"] == name:
                return int(row["bks"]), int(row["k"])
    return None, None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("instance", help="path to .vrp file")
    ap.add_argument("--seconds", type=float, default=5)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args()

    path = Path(a.instance)
    name = path.stem
    capacity, coords, demands = parse_vrp(path)
    n = len(coords) - 1

    model = pyvrp.read(path, round_func="round")
    # PyVRP defaults to one vehicle per client when the file gives no fleet size.
    if model.num_vehicles < n:
        sys.exit(f"{name}: only {model.num_vehicles} vehicles for {n} clients")
    res = pyvrp.solve(model, stop=MaxRuntime(a.seconds), seed=a.seed,
                      display=False)
    sol = res.best
    # Clients 1..n follow file order (depot is 0).
    # In schedules, a client's idx is 0-based among clients (depot is separate).
    routes = [[v.idx + model.num_depots for v in r.schedule() if v.is_client()]
              for r in sol.routes()]

    recomputed = sum(route_cost(r, coords) for r in routes)
    visited = sorted(c for r in routes for c in r)
    ok_cover = visited == list(range(1, n + 1))
    ok_load = all(sum(demands[c] for c in r) <= capacity for r in routes)
    pyvrp_cost = int(sol.distance())
    bks, k = find_bks(name)
    gap = (recomputed - bks) / bks * 100 if bks else float("nan")
    ok = ok_cover and ok_load and recomputed == pyvrp_cost and sol.is_feasible()

    print(f"{name}: pyvrp_cost={pyvrp_cost} recomputed={recomputed} "
          f"routes={len(routes)} k={k} bks={bks} gap_pct={gap:.4f} "
          f"cover={ok_cover} load={ok_load} feasible={sol.is_feasible()} "
          f"vehicles_available={model.num_vehicles} "
          f"{'OK' if ok else 'FAIL'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
