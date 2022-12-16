# img-convert (Pam)
Processes a given picture of a maze by splitting it into 3 parts (the map, start and end). The example is self-explanatory.

## How does it work?

### The original image

<img height="200" src="https://github.com/hendrychjan/kevin/blob/main/examples/img-convert/maze.png?raw=true">

> _(maze.png)_

### Only the maze map:

<img height="160" src="https://github.com/hendrychjan/kevin/blob/main/examples/img-convert/map.png?raw=true">

> _(out/map.png)_

### Start:

<img height="160" src="https://github.com/hendrychjan/kevin/blob/main/examples/img-convert/start.png?raw=true">

> _(out/start.png)_

### End:

<img height="160" src="https://github.com/hendrychjan/kevin/blob/main/examples/img-convert/end.png?raw=true">

> _(out/end.png)_

## How to use it?

> This is a shell script, so it won't work in windows (or use WSL)

- check if you have `imagemagick` installed

```
magick --version
```

- make `convert.sh` executable

```
chmod +x ./convert.sh
```

- replace the example `maze.png` with your own image

- run the scrit (the results are then placed to `out/`)

```
./convert.sh
```
