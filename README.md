# INF1018

## Ambiente com Docker

Este projeto pode ser compilado em um container Ubuntu com `gcc`, sem instalar compiladores diretamente no sistema operacional.

## macOS

1. Instale o Docker Desktop:
   https://www.docker.com/products/docker-desktop/
2. Abra o Docker Desktop e espere o servico iniciar.
3. No terminal, entre na pasta do projeto e execute:

```bash
docker pull ubuntu
docker run -it --rm -v "$(pwd):/app" -w /app ubuntu bash
apt update
apt install gcc -y
```

## Windows

1. Instale o Docker Desktop:
   https://www.docker.com/products/docker-desktop/
2. Abra o Docker Desktop e espere o servico iniciar.
3. No PowerShell, entre na pasta do projeto e execute:

```powershell
docker pull ubuntu
docker run -it --rm -v "${PWD}:/app" -w /app ubuntu bash
apt update
apt install gcc -y
```
