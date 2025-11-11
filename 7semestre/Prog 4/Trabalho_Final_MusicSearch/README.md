# Integração com API Pública e Cache de Dados
- OBS: Como nunca tinha feito um arquivo .md usei o da Biia como base, porém está tudo atualizado de acordo com o meu projeto!

## Descrição do Projeto
Este projeto visa desenvolver uma aplicação web utilizando o framework Flask e o banco de dados SQLite, com integração a uma API pública selecionada. A aplicação terá funcionalidades de busca e exibição de dados fornecidos pela API, além de implementar um sistema de cache local para otimizar o desempenho em consultas subsequentes. Assim, uma vez que um recurso seja recuperado da API, ele será armazenado no banco de dados local. Dessa forma, consultas futuras ao mesmo recurso poderão ser respondidas diretamente pelo banco de dados, reduzindo a necessidade de requisições repetidas à API externa e melhorando a eficiência da aplicação.E os usuários cadastrados vão poder favoritar suas músicas preferidas em seu perfil.

## API Pública
A API do Deezer permite buscar músicas, álbuns, artistas e playlists disponíveis na plataforma Deezer. Ao fazer uma requisição HTTP para o endpoint https://api.deezer.com/search, é possível pesquisar e obter informações detalhadas sobre conteúdos musicais como Nome, Ano, Artista, Capa do albúm e Através do Preview o usuário tem uma prévia de 30 segundos da música com base em palavras-chave fornecidas pelo usuário. Na opção dos 3 pontos tem como mudar a velocidade de reprodução da prévia e tem como baixar também.

## Funcionalidades
### Busca de Dados
- Os usuários podem buscar músicas, artistas ou álbuns usando um formulário na interface em Buscar música.
- A aplicação verifica se os dados solicitados já estão armazenados no banco de dados local (cache). Se os dados estiverem - disponíveis, eles são recuperados diretamente do banco.
- Caso os dados não estejam armazenados, a aplicação faz uma requisição à API do Deezer, exibe os dados ao usuário e os armazena no banco de dados para futuras consultas.
- caso pesquise uma música e não apareça a que você queira, pesquisa a música junto com o nome do artista.

## Cache de Dados
- A aplicação implementa um sistema de cache que verifica se os dados já estão no banco de dados local antes de fazer novas requisições à API do Deezer.
- Os dados armazenados são reutilizados em futuras requisições, reduzindo o número de chamadas à API e melhorando a performance da aplicação.

## Sistema de Favoritos
Usuários autenticados podem favoritar músicas.
Cada usuário tem sua própria lista de favoritos, que pode ser visualizada na página "Perfil".

## Requisitos Técnicos
- Flask: Framework web para desenvolvimento da aplicação.
- SQLite: Banco de dados local para armazenar dados da aplicação.
- Jinja2: Sistema de templates para renderizar as páginas da aplicação.
- JavaScript: Para implementar requisições assíncronas e melhorar a interatividade da aplicação.

## Autenticação
- Foi implementado um sistema de autenticação com login e registro de usuários, garantindo a segurança das senhas por meio de hashing.


### Passos para Execução

1. **Navegue até o diretório do projeto:**:
   cd <nome-do-diretorio>

2. **Crie um ambiente virtual**:
   python3 -m venv flask

3. **Ative o ambiente virtual**:
   source flask/bin/activate

4. **Instale as dependências do projeto**:
   pip install -r requirements.txt

5. **Configure a variável de ambiente para o Flask**:
   export FLASK_APP=flasky.py

6. **Acesse o shell do Flask**:
   flask shell

7. **Crie as tabelas no banco de dados**:
   db.create_all()

8. **Saia do shell do Flask**:
   exit()

9. **Execute a aplicação**:
   flask run

10. **Acesse a aplicação em seu navegador**:
   Abra http://127.0.0.1:5000/ em seu navegador