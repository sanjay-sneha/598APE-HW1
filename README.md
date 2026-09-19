# Artifact for Raytracer Optimizations

Here we have instructions explaining how to build the code, reproduce our baseline and optimized results, and evaluate each individual optimization.

We split work across two branches:
- `main` — Sneha's optimizations
- `naomi-perf` — Naomi's optimizations

---

## 1. Build Instructions

### Requirements
- A working C++ compiler
- `make`
- ImageMagick (for importing/exporting non-PPM images)
- FFmpeg/IINA/other software (for exporting movies from image sequences)

All of these are pre-installed in the provided Docker image or can be built locally.

### Build
```bash
make -j
```

### Clean
```bash
make clean
```

---

## 2. Running and Benchmarking

The raytracer prints its own timing after each run (timer code wraps the main computational loop in `main.cpp`), in the form:
```
Total time to create images=X.XXXXXX seconds
```
This is the number we report throughout our paper; no external timing tool is needed to reproduce our numbers, though we also used `perf` for profiling (see below).

### Piano Room (image)
```bash
./main.exe -i inputs/pianoroom.ray --ppm -o output/pianoroom.ppm -H 500 -W 500
```

### Elephant Mesh (video)
```bash
./main.exe -i inputs/elephant.ray --ppm -a inputs/elephant.animate --movie -F 24 -W 500 -H 500 -o output/elephant.mp4
```

### Verifying baseline vs. optimized
To reproduce our baseline numbers, check out the commit immediately before any optimization commits (see table below) and rebuild with `make clean && make -j` before timing. To verify a specific optimization's effect in isolation, check out the commit *after* that optimization's change and rebuild the same way. This way you can compare the two printed times to reproduce the delta we reported in the corresponding section.

### Profiling with perf
To reproduce the profiler output referenced in our report:
```
make clean 

make -j

perf record -g -o perf-baseline.data \
./main.exe -i inputs/pianoroom.ray --ppm \
-o output/pianoroom.ppm -H 500 -W 500

perf report -i perf-baseline.data

perf record -g -o perf-elephant.data \
./main.exe -i inputs/elephant.ray --ppm \
-o output/elephant.ppm -H 500 -W 500

perf report -i perf-elephant.data
```

Swap the input file/flags for the elephant mesh scene as needed to reproduce the elephant scene profiles.

---

## 3. Optimizations and How to Evaluate Each

For each optimization, check out the listed commit (on the listed branch), rebuild (`make clean && make -j`), and re-run the relevant scene command above. Comparing the printed render time to the previous row reproduces the speedup reported in that section of our paper.

To evaluate any of these in isolation, check out the listed commit directly, rebuild, and compare timing against the immediately preceding commit on that branch.
