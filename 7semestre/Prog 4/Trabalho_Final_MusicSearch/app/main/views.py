from flask import render_template, redirect, url_for, flash, request, jsonify
from . import main
import requests
from flask_login import login_required, current_user
from ..models import User, Music, Favorite
from .. import db

@main.route('/', methods=['GET', 'POST'])
def index():
    return render_template('index.html')

@main.route('/user/<username>')
@login_required
def user(username):
    user = User.query.filter_by(username=username).first_or_404()
    favorites = get_user_favorites()  # Obtém os favoritos do usuário logado
    return render_template('user.html', user=user, favorites=favorites)

@main.route('/busca_musica')
def busca():
    return render_template('busca_musica.html')

def get_music_from_db(music_title):
    return Music.query.filter_by(titulo=music_title).first()

@main.route('/music', methods=['POST'])
def get_music_info():
    music_title = request.form.get('music')
    if not music_title:
        return jsonify({'error': 'Nome da música não informado.'}), 400

    # Verificar se a música já existe no banco de dados local
    music = get_music_from_db(music_title)
    if music:
        return jsonify({
            'id': music.id,
            'titulo': music.titulo,
            'ano': music.ano,
            'artistas': music.artistas,
            'poster_url': music.poster_url,
            'preview_url': music.preview_url
        })

    # Se não estiver no banco, fazer a requisição à API externa
    api_url = f"https://api.deezer.com/search?q={music_title}"
    try:
        response = requests.get(api_url)
        response.raise_for_status()
    except requests.RequestException as e:
        return jsonify({'error': 'Erro ao buscar informações da música: ' + str(e)}), 500

    results = response.json().get('data', [])
    if not results:
        return jsonify({'error': 'Nenhuma música encontrada.'}), 404

    music_data = results[0]
    music_info = {
        'titulo': music_data.get('title', 'N/A'),
        'ano': music_data.get('release_date', 'N/A').split("-")[0] if music_data.get('release_date') else 'N/A',
        'artistas': music_data['artist']['name'] if music_data.get('artist') else 'N/A',
        'poster_url': music_data['album']['cover_medium'] if music_data.get('album') else 'N/A',
        'preview_url': music_data.get('preview', 'N/A')
    }

    # Salvar a música no banco de dados e retornar os dados
    music = save_music_to_db(music_info)
    music_info['id'] = music.id
    return jsonify(music_info)

# Função para salvar uma música no banco de dados local, evitando duplicatas
def save_music_to_db(music_info):
    # Verificar se a música já existe no banco de dados com base no título
    existing_music = Music.query.filter_by(titulo=music_info['titulo']).first()
    if existing_music:
        return existing_music  # Retorna a música existente para evitar duplicatas

    # Se a música não existe, cria uma nova entrada
    new_music = Music(
        titulo=music_info['titulo'],
        ano=music_info.get('ano'),
        artistas=music_info.get('artistas'),
        poster_url=music_info.get('poster_url'),
        preview_url=music_info.get('preview_url')
    )
    db.session.add(new_music)
    db.session.commit()
    return new_music



# Endpoint para alternar a música entre favorita e não favorita
@main.route('/favorite/music/<int:music_id>', methods=['POST'])
@login_required
def favorite_music(music_id):
    music = Music.query.get(music_id)
    if not music:
        return jsonify({'error': 'Música não encontrada.'}), 404

    # Verificar se a música já está nos favoritos
    favorite = Favorite.query.filter_by(user_id=current_user.id, music_id=music.id).first()
    
    if favorite:
        # Se a música já está nos favoritos, remove-a
        db.session.delete(favorite)
        message = 'Música removida dos favoritos.'
    else:
        # Caso contrário, adiciona aos favoritos
        new_favorite = Favorite(user_id=current_user.id, music_id=music.id)
        db.session.add(new_favorite)
        message = 'Música adicionada aos favoritos.'

    db.session.commit()
    return jsonify({'message': message})

# Função para obter músicas favoritas do usuário logado
def get_user_favorites():
    favorites = Favorite.query.filter_by(user_id=current_user.id).all()
    return [favorite.music for favorite in favorites]

