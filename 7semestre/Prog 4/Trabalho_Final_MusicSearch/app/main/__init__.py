from flask import Blueprint

main = Blueprint('main', __name__)

from . import views, errors #import após instanciação de main para evitar referência circular