## When building Qt
1. Optimize for size instead of speed

**`-optimize-size`** or use **`-O1`** instead of **`O2`**, **`Ox`** or **`O3`**

2. Skip unneeded repositories

**`-skip <repo name>`**

Qt 5.12 example:

3. Disable unneeded features

**`-no-feature-<feature name>`**

Qt 5.12 example:

4. Disable Whole Program Optimization/Link Time Code Generation

Default. No actions needed.

## When building FFmpeg
1. Optimize for size instead of speed

**`--enable-small`**

2. Disable unneeded features

See <url> for more information.

## When building this project
1. Optimize for size instead of speed

**`CONFIG *= enable_small`**

2. Enable Whole Program Optimization/Link Time Code Generation

**`CONFIG *= enable_ltcg`**
