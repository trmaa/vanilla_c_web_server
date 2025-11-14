# Web Server/API hoster...

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
serve <port> <dir>
```

Example:
```bash 
serve 8080 ./assets/
firefox http://localhost:8080/index.html
```

## Features to do:

It lacks url encoding, file trees for directorys, querys...
