# Importar bibliotecas necessárias
from datasets import load_dataset
import pandas as pd
from surprise import Dataset, Reader, SVD, accuracy
from surprise.model_selection import train_test_split

# 1. Carregar o dataset do Hugging Face
dataset = load_dataset("celsowm/adoro_cinema_filmes")

# 2. Explorar o dataset e verificar as primeiras linhas
df = pd.DataFrame(dataset['train'])  # Converter o dataset Hugging Face em um DataFrame
print(df.head())  # Ver as primeiras linhas do dataset

# Selecionar as colunas relevantes (ajuste de acordo com o dataset)
df = df[['user_id', 'movie_id', 'rating']]  # Ajustar se os nomes das colunas forem diferentes

# Verificar se há valores ausentes e tratá-los se necessário
print(df.isnull().sum())
df.dropna(inplace=True)  # Remover valores ausentes

# 3. Dividir os dados em treino e teste
train_data, test_data = train_test_split(df, test_size=0.2, random_state=42)

# Definir o formato dos dados para o Surprise
reader = Reader(rating_scale=(1, 5))  # Assumindo que as notas vão de 1 a 5

# Carregar os dados no formato surprise
train_data_surprise = Dataset.load_from_df(train_data[['user_id', 'movie_id', 'rating']], reader)

# Dividir os dados em treino e teste no formato Surprise
trainset, testset = train_test_split(train_data_surprise, test_size=0.2)

# 4. Criar e treinar o modelo SVD
algo = SVD()  # Singular Value Decomposition (SVD)
algo.fit(trainset)  # Treinar o modelo com os dados de treino

# 5. Fazer previsões e avaliar o modelo
predictions = algo.test(testset)  # Prever no conjunto de teste
rmse = accuracy.rmse(predictions)  # Calcular o RMSE (erro quadrático médio)

# Fazer uma previsão para um exemplo de usuário e filme
user_id = 1  # Exemplo de um usuário
movie_id = 500  # Exemplo de um filme
pred = algo.predict(user_id, movie_id)
print(f"Previsão da nota do usuário {user_id} para o filme {movie_id}: {pred.est}")

# Avaliação final: você pode adicionar mais métricas, como precisão e recall, para refinar a análise.
