# HTTP Server

Its a small vanilla C very basic program which lets you host html or whatever file on localhost.

## Instalation:

```bash
git clone https://github.com/trmaa/vanilla_c_web_server.git
cd vanilla_c_web_server
chmod +x ./scripts/make.sh
./scripts/make.sh build install
```

## Use:

```bash 
serve [-p port] [-d directory]
```

Example:
```bash 
serve -d example/
firefox http://localhost:8080/index.html
```

## Features to add and flaws:

TODO:
- It lacks url encoding, 
- file trees for directorys, 
- querys...

FLAWS:
- It handles text responces of up to 8kB, so it core dumps.
- It also core dumps for no reason some times.
