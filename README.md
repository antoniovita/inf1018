# INF1018

## Uso com Docker

Os binarios presentes no repositorio foram gerados para Linux ARM e nao executam diretamente em outros ambientes, como macOS. Se quiser reproduzir o ambiente usando Docker:

```bash
docker pull ubuntu
docker run -it --rm -v "$(pwd):/app" -w /app ubuntu bash
apt update
apt install gcc -y
```
