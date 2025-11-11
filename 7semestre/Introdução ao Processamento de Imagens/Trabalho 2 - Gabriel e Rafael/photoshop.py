# Trabalho 2 de IPI
# Gabriel Gomes Marchesan
# Rafael Nalin Alves

import sys
import cv2
import numpy as np
from PyQt5.QtWidgets import (QApplication, QMainWindow, QLabel, QAction, QFileDialog,
                             QMessageBox, QScrollArea, QDockWidget, QWidget,
                             QVBoxLayout, QHBoxLayout, QPushButton, QSlider, 
                             QFormLayout, QStyle, QProgressDialog)
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt

# --- ESTILO DA APLICAÇÃO (TEMA ESCURO) ---
estilo_escuro = """
    QMainWindow { background-color: #2b2b2b; }
    QMenuBar { background-color: #3c3c3c; color: #f0f0f0; }
    QMenuBar::item:selected { background-color: #0078d7; }
    QMenu { background-color: #3c3c3c; color: #f0f0f0; border: 1px solid #4f4f4f; }
    QMenu::item:selected { background-color: #0078d7; }
    QDockWidget { background-color: #3c3c3c; color: #f0f0f0; }
    QDockWidget::title { text-align: left; background: #4f4f4f; padding: 5px; }
    QWidget { background-color: #3c3c3c; color: #f0f0f0; }
    QLabel { color: #f0f0f0; }
    QPushButton { background-color: #0078d7; color: white; border-radius: 4px; padding: 8px; border: 1px solid #005a9e; }
    QPushButton:hover { background-color: #005a9e; }
    QPushButton:pressed { background-color: #003a6e; }
    QSlider::groove:horizontal { border: 1px solid #4f4f4f; height: 8px; background: #2b2b2b; margin: 2px 0; border-radius: 4px; }
    QSlider::handle:horizontal { background: #0078d7; border: 1px solid #005a9e; width: 18px; margin: -5px 0; border-radius: 9px; }
    QScrollArea { border: none; }
"""

class EditorDeImagensApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.imagem_original = None            # Carrega a imagem 
        self.imagem_processada = None          # A imagem atual após o processamento
        self.configuracao_filtro_ativo = None  # Configuração do filtro
        self.imagem_antes_do_filtro = None     # Backup da Imagem antes de aplicar o filtro
        
        # Pilhas para histórico de alterações (desfazer/refazer)
        self.pilha_historico = []
        self.pilha_refazer = []

        # Inicializa a interface Gráfica
        self.iniciar_ui()

    # SEÇÃO DE CONFIGURAÇÃO DA INTERFACE GRÁFICA
    def iniciar_ui(self):
        # Configura a janela principal, a área de imagem e o painel de controles.
        self.setWindowTitle('PhotoPy - Editor de Imagens')
        self.setGeometry(100, 100, 1200, 800)
        
        # Ele mostra a imagem atual ou a mensagem inicial
        self.rotulo_imagem = QLabel("Abra uma imagem para começar (Arquivo > Abrir(Ctrl + o))")
        fonte = self.rotulo_imagem.font(); fonte.setPointSize(16); self.rotulo_imagem.setFont(fonte)
        self.rotulo_imagem.setAlignment(Qt.AlignCenter)
        self.rotulo_imagem.setStyleSheet("color: #7f7f7f;")
        
        # Quando imagem está maior que o espaço ele cria uma barra de rolagem
        self.area_rolagem = QScrollArea()
        self.area_rolagem.setWidget(self.rotulo_imagem)
        self.area_rolagem.setWidgetResizable(True)
        self.setCentralWidget(self.area_rolagem)
        
        # Painel de controle dos filtros
        self.painel_controles = QDockWidget('Painel de Controle', self)
        self.painel_controles.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
        self.widget_controles = QWidget()
        self.layout_controles = QVBoxLayout(self.widget_controles)
        self.painel_controles.setWidget(self.widget_controles)
        self.addDockWidget(Qt.RightDockWidgetArea, self.painel_controles)
        self.painel_controles.setVisible(False)
        
        # Cria o menu de barra superior
        self.criar_menus()

    # --- Menu de barra superior
    def criar_menus(self):

        # Cria e popula a barra de menus da aplicação.
        barra_menus = self.menuBar()
        estilo = self.style()

        # Menu de Arquivos
        menu_arquivo = barra_menus.addMenu('Arquivo')
        acao_abrir = QAction(estilo.standardIcon(QStyle.SP_DirOpenIcon), 'Abrir Imagem...', self); acao_abrir.setShortcut('Ctrl+O'); acao_abrir.triggered.connect(self.abrir_imagem); menu_arquivo.addAction(acao_abrir)
        acao_salvar = QAction(estilo.standardIcon(QStyle.SP_DialogSaveButton), 'Salvar Imagem...', self); acao_salvar.setShortcut('Ctrl+S'); acao_salvar.triggered.connect(self.salvar_imagem); menu_arquivo.addAction(acao_salvar)
        menu_arquivo.addSeparator()
        acao_reverter = QAction('Reverter para Original', self); acao_reverter.triggered.connect(self.reverter_para_original); menu_arquivo.addAction(acao_reverter)
        acao_sair = QAction(estilo.standardIcon(QStyle.SP_TitleBarCloseButton), 'Sair', self); acao_sair.triggered.connect(self.close); menu_arquivo.addAction(acao_sair)

        # Menu de editar 
        menu_editar = barra_menus.addMenu('Editar')
        self.acao_desfazer = QAction(estilo.standardIcon(QStyle.SP_ArrowBack), 'Desfazer', self); self.acao_desfazer.setShortcut('Ctrl+Z'); self.acao_desfazer.setEnabled(False); self.acao_desfazer.triggered.connect(self.desfazer_acao); menu_editar.addAction(self.acao_desfazer)
        self.acao_refazer = QAction(estilo.standardIcon(QStyle.SP_ArrowForward), 'Refazer', self); self.acao_refazer.setShortcut('Ctrl+Y'); self.acao_refazer.setEnabled(False); self.acao_refazer.triggered.connect(self.refazer_acao); menu_editar.addAction(self.acao_refazer)

        # Menu de ajustes
        menu_ajustes = barra_menus.addMenu('Ajustes')
        acao_negativo = QAction('Negativo', self); acao_negativo.triggered.connect(self.aplicar_negativo); menu_ajustes.addAction(acao_negativo)
        acao_brilho_hsi = QAction('Brilho e Contraste (HSI)...', self); acao_brilho_hsi.triggered.connect(self.mostrar_controles_brilho_contraste_hsi); menu_ajustes.addAction(acao_brilho_hsi)
        acao_gamma = QAction('Correção Gamma...', self); acao_gamma.triggered.connect(self.mostrar_controles_gamma); menu_ajustes.addAction(acao_gamma)
        acao_equalizar = QAction('Equalizar Histograma', self); acao_equalizar.triggered.connect(self.aplicar_equalizacao_hist); menu_ajustes.addAction(acao_equalizar)
        menu_ajustes.addSeparator()
        acao_limiar = QAction("Limiarização...", self); acao_limiar.triggered.connect(self.mostrar_controles_limiar); menu_ajustes.addAction(acao_limiar)

        # Menu de Redimensionar
        menu_redimensionar = barra_menus.addMenu('Redimensionar')
        acao_vizinho = QAction('Vizinho Mais Próximo...', self); acao_vizinho.triggered.connect(self.mostrar_controles_redim_vizinho); menu_redimensionar.addAction(acao_vizinho)
        acao_red_bilinear = QAction('Bilinear...', self); acao_red_bilinear.triggered.connect(self.mostrar_controles_redim_bilinear); menu_redimensionar.addAction(acao_red_bilinear)

        # Menu de Filtros Espaciais
        menu_filtros_espaciais = barra_menus.addMenu('Filtros Espaciais')
        menu_suavizacao = menu_filtros_espaciais.addMenu('Suavização')
        acao_filtro_caixa = QAction('Filtro de Caixa...', self); acao_filtro_caixa.triggered.connect(self.mostrar_controles_filtro_caixa); menu_suavizacao.addAction(acao_filtro_caixa)
        acao_filtro_gaussiano = QAction('Filtro Gaussiano...', self); acao_filtro_gaussiano.triggered.connect(self.mostrar_controles_gaussiano); menu_suavizacao.addAction(acao_filtro_gaussiano)
        menu_agucamento = menu_filtros_espaciais.addMenu('Aguçamento')
        acao_agucar_laplaciano = QAction('Com Laplaciano...', self); acao_agucar_laplaciano.triggered.connect(self.mostrar_controles_agucar_laplaciano); menu_agucamento.addAction(acao_agucar_laplaciano)
        acao_agucar_gradiente = QAction('Com Gradiente...', self); acao_agucar_gradiente.triggered.connect(self.mostrar_controles_agucar_gradiente); menu_agucamento.addAction(acao_agucar_gradiente)
        
        # Menu de Frequência
        menu_freq = barra_menus.addMenu('Frequência')
        acao_suav_freq = QAction('Suavização (Butterworth)...', self); acao_suav_freq.triggered.connect(self.mostrar_controles_suavizacao_freq); menu_freq.addAction(acao_suav_freq)
        acao_aguc_freq = QAction('Aguçamento (Butterworth)...', self); acao_aguc_freq.triggered.connect(self.mostrar_controles_agucamento_freq); menu_freq.addAction(acao_aguc_freq)
        menu_freq.addSeparator()
        acao_ver_espectro = QAction('Visualizar Espectro de Fourier', self); acao_ver_espectro.triggered.connect(self.visualizar_espectro_fourier); menu_freq.addAction(acao_ver_espectro)
        
        # Menu de Morfologia
        menu_morfologia = barra_menus.addMenu('Morfologia')
        acao_erosao = QAction('Erosão...', self); acao_erosao.triggered.connect(self.mostrar_controles_erosao); menu_morfologia.addAction(acao_erosao)
        acao_dilatacao = QAction('Dilatação...', self); acao_dilatacao.triggered.connect(self.mostrar_controles_dilatacao); menu_morfologia.addAction(acao_dilatacao)
        
    # --- Constrói dinamicamente o painel de controles com sliders para parâmetros de filtros ---
    def construir_controles(self, configuracao):                    
        if not self._verificar_imagem_carregada(): return             # Verifica se tem uma imagem carregada
        self.limpar_layout(self.layout_controles)                     # Limpa os controles anteriores
        self.configuracao_filtro_ativo = configuracao                 # Ele armazena a configuração do filtro atual 
        self.imagem_antes_do_filtro = self.imagem_processada.copy()   # Backup para desfazer/cancelar
        layout_formulario = QFormLayout()                             # Layout para sliders(componentes da interface gráfica) e rótulos
        self.sliders, self.rotulos_valores = [], []                   # Lista de slides e de rotulos de valores
        
        # Cria slider para cada parâmetro na configuração
        for param in configuracao['params']:
            slider = QSlider(Qt.Horizontal)
            rotulo_valor = QLabel(str(param['initial']))
            slider.setRange(param['min'], param['max']); slider.setValue(param['initial'])
            if 'step' in param: slider.setSingleStep(param['step'])
            
            # Conecta eventos do slider para atualização de label e preview
            slider.valueChanged.connect(self.atualizar_valores_label)
            slider.sliderReleased.connect(self.atualizar_preview_tempo_real)

            self.sliders.append(slider); self.rotulos_valores.append(rotulo_valor)
            layout_linha = QHBoxLayout(); layout_linha.addWidget(slider); layout_linha.addWidget(rotulo_valor, alignment=Qt.AlignRight)
            layout_formulario.addRow(f"<b>{param['name']}</b>", layout_linha)

        # Adiciona formulário ao painel  
        self.layout_controles.addLayout(layout_formulario)
        
        # Botões Aplicar e Cancelar
        botao_aplicar, botao_cancelar = QPushButton("Aplicar"), QPushButton("Cancelar")
        botao_aplicar.clicked.connect(self.aplicar_mudancas_filtro)
        botao_cancelar.clicked.connect(self.cancelar_mudancas_filtro)
        layout_botoes = QHBoxLayout(); layout_botoes.addStretch(); layout_botoes.addWidget(botao_cancelar); layout_botoes.addWidget(botao_aplicar)
        self.layout_controles.addLayout(layout_botoes); self.layout_controles.addStretch()
        
        # Define título do painel e o torna visível 
        self.painel_controles.setWindowTitle(configuracao['window_title']); self.painel_controles.setVisible(True)
        self.atualizar_valores_label() # Atualiza labels iniciais

    # Ele é essencial para que os ajustes na interface afetem a imagem corretamente.
    def obter_parametros_atuais(self):
        params = {} # Cria um dicionário para armazenar os valores dos sliders
        if not self.configuracao_filtro_ativo: return params # Vai vereficar se existe um filtro ativo, caso não esteja ele retorna o dicionário vazio
        for i, config_slider in enumerate(self.configuracao_filtro_ativo['params']): # Inicia um loop que percorre a lista de parâmetros definida
            valor = self.sliders[i].value() # Pega o valor atual do slider na posição 'i' da lista
            valor_exibido = valor # Cria uma cópia do valor inicial.
            if config_slider.get('type') == 'float': # Verifica se o parâmetro deve ser um número decimal (float)
                valor_exibido = valor / config_slider.get('divisor', 1.0)
            if config_slider.get('odd', False) and valor_exibido % 2 == 0 and valor_exibido > 0:
                valor_exibido += 1 # Garante tamanho de kernel ímpar
            params[config_slider['id']] = valor_exibido # Adiciona o valor ajustado ao dicionário 'params' usando a chave 'id' do parâmetro
            texto_valor = f'{valor_exibido:.2f}' if isinstance(valor_exibido, float) else str(valor_exibido)
            self.rotulos_valores[i].setText(texto_valor)
        return params # Retorna o dicionário 'params' com todos os valores dos sliders, pronto para ser usado

    # Atualiza apenas os rótulos de valores dos sliders.
    def atualizar_valores_label(self):
        self.obter_parametros_atuais()

    # Aplica o filtro com parâmetros atuais e exibe preview temporário
    def atualizar_preview_tempo_real(self):
        if self.configuracao_filtro_ativo:
            params = self.obter_parametros_atuais()
            imagem_preview = self.configuracao_filtro_ativo['process_function'](self.imagem_antes_do_filtro, **params)
            self.exibir_imagem(imagem_preview)

    # Aplica o filtro permanentemente, adiciona ao histórico e fecha o painel
    def aplicar_mudancas_filtro(self):
        if not self.configuracao_filtro_ativo: return
        
        progresso = QProgressDialog("Processando imagem...", "Cancelar", 0, 0, self)
        progresso.setWindowModality(Qt.WindowModal); progresso.show(); QApplication.processEvents()

        self.adicionar_ao_historico() # Salva estado atual no histórico
        params = self.obter_parametros_atuais()
        self.imagem_processada = self.configuracao_filtro_ativo['process_function'](self.imagem_antes_do_filtro, **params)
        
        progresso.close() # Fecha painel
        self.exibir_imagem(self.imagem_processada)
        self.ocultar_e_limpar_controles()

    # Reverte para imagem antes do filtro e fecha o painel
    def cancelar_mudancas_filtro(self):
        self.exibir_imagem(self.imagem_antes_do_filtro)
        self.ocultar_e_limpar_controles()

    # Oculta o painel e reseta configurações ativas
    def ocultar_e_limpar_controles(self):
        self.painel_controles.setVisible(False); self.limpar_layout(self.layout_controles)
        self.configuracao_filtro_ativo, self.imagem_antes_do_filtro = None, None

    # Adiciona cópia da imagem processada ao histórico de desfazer. Limpa pilha de refazer e atualiza botões.
    def adicionar_ao_historico(self):
        if self.imagem_processada is not None:
            self.pilha_historico.append(self.imagem_processada.copy())
            self.pilha_refazer.clear()
            self.atualizar_botoes_historico()

    # Desfaz a última alteração, movendo para pilha de refazer. Atualiza exibição e botões.
    def desfazer_acao(self):
        if not self.pilha_historico: return
        self.pilha_refazer.append(self.imagem_processada.copy())
        self.imagem_processada = self.pilha_historico.pop()
        self.exibir_imagem(self.imagem_processada)
        self.atualizar_botoes_historico()

    # Refaz a alteração desfeita, movendo de volta para histórico. Atualiza exibição e botões.
    def refazer_acao(self):
        if not self.pilha_refazer: return
        self.pilha_historico.append(self.imagem_processada.copy())
        self.imagem_processada = self.pilha_refazer.pop()
        self.exibir_imagem(self.imagem_processada)
        self.atualizar_botoes_historico()
        
    # Atualiza estado (habilitado/desabilitado) dos botões Desfazer/Refazer.
    def atualizar_botoes_historico(self):
        self.acao_desfazer.setEnabled(len(self.pilha_historico) > 0)
        self.acao_refazer.setEnabled(len(self.pilha_refazer) > 0)
        
    # Verifica se há imagem carregada; exibe aviso se não
    def _verificar_imagem_carregada(self):
        if self.imagem_original is None: 
            QMessageBox.warning(self, 'Aviso', 'Nenhuma imagem carregada.')
            return False
        return True

    # ------------------------------------------- SEÇÃO DE FUNÇÕES AUXILIARES PARA PROCESSAMENTO --------------------------------------------------

    # Ele transforma uma imagem colorida (com vermelho, verde e azul - RGB) em uma imagem
    # em tons de cinza, usando uma combinação ponderada dos canais de cor.
    def _para_cinza_numpy(self, imagem):
        if len(imagem.shape) < 3: return imagem # Já é cinza
        return np.dot(imagem[...,:3], [0.114, 0.587, 0.299]).astype(np.uint8) 
        
    # Utilizado na correção gamma
    def _aplicar_lut(self, imagem, lut):
        return lut[imagem]

    # Brilho e contraste
    def _rgb_para_hsi(self, imagem_rgb):
        with np.errstate(divide='ignore', invalid='ignore'):
            imagem_float = imagem_rgb.astype(np.float32) / 255.0 # HSI trabalha melhor com float
            R, G, B = imagem_float[..., 0], imagem_float[..., 1], imagem_float[..., 2]
            I = (R + G + B) / 3.0 # calculo de brilho
            min_rgb = np.minimum(np.minimum(R, G), B) # saturação
            S = 1 - min_rgb / (I + 1e-10) # Calcula a saturação (S), que mede a "pureza" da cor (0 = cinza, 1 = cor pura).
            S[I == 0] = 0 # Corrige a saturação quando a intensidade (I) é 0
            num = 0.5 * ((R - G) + (R - B)) # Calcula o numerador para o ângulo theta
            den = np.sqrt((R - G)**2 + (R - B)*(G - B)) # Calcula o denominador para theta
            theta = np.arccos(np.clip(num / (den + 1e-10), -1, 1)) 
            H = np.where(B <= G, theta, 2 * np.pi - theta) / (2 * np.pi) # Calcula o matiz (H), que representa a tonalidade da cor
            return np.stack((H, S, I), axis=-1) # Combina os canais H, S e I em uma única array de 3 dimensões (H, S, I para cada pixel)

    # Após ajustar brilho ou contraste no espaço HSI, para retornar a uma imagem que pode ser exibida ou salva
    def _hsi_para_rgb(self, imagem_hsi):
        H, S, I = imagem_hsi[..., 0] * 360, imagem_hsi[..., 1], imagem_hsi[..., 2]
        R, G, B = np.zeros_like(H), np.zeros_like(H), np.zeros_like(H)
        idx_rg, idx_gb, idx_br = (H >= 0) & (H < 120), (H >= 120) & (H < 240), (H >= 240) & (H <= 360)
        # Calcula RGB para o setor RG (0° a 120°):
        H_rad = np.deg2rad(H[idx_rg]); B[idx_rg] = I[idx_rg]*(1-S[idx_rg]); R[idx_rg] = I[idx_rg]*(1+(S[idx_rg]*np.cos(H_rad))/(np.cos(np.deg2rad(60)-H_rad)+1e-10)); G[idx_rg] = 3*I[idx_rg]-(R[idx_rg]+B[idx_rg])
        # Calcula RGB para o setor GB (120° a 240°):
        H_rad = np.deg2rad(H[idx_gb]-120); R[idx_gb] = I[idx_gb]*(1-S[idx_gb]); G[idx_gb] = I[idx_gb]*(1+(S[idx_gb]*np.cos(H_rad))/(np.cos(np.deg2rad(60)-H_rad)+1e-10)); B[idx_gb] = 3*I[idx_gb]-(R[idx_gb]+G[idx_gb])
        # Calcula RGB para o setor BR (240° a 360°):
        H_rad = np.deg2rad(H[idx_br]-240); G[idx_br] = I[idx_br]*(1-S[idx_br]); B[idx_br] = I[idx_br]*(1+(S[idx_br]*np.cos(H_rad))/(np.cos(np.deg2rad(60)-H_rad)+1e-10)); R[idx_br] = 3*I[idx_br]-(G[idx_br]+B[idx_br])
        rgb_img = np.stack((R, G, B), axis=-1) # Combina os canais R, G e B em uma única array de 3 dimensões (altura, largura, 3)
        return np.clip(rgb_img * 255, 0, 255).astype(np.uint8) # Converte os valores de volta para o intervalo 0-255

    # filtros espaciais ( )
    def _convolucao_manual_2d(self, imagem, kernel):
        imagem_float = imagem.astype(np.float64) # Converte para decimais. Isso é feito para evitar perdas de precisão durante os cálculos da convolução
        alt_img, larg_img = imagem.shape[:2]; alt_kernel, larg_kernel = kernel.shape # Pega as dimensões da imagem e do kernel
        pad_alt, pad_larg = alt_kernel // 2, larg_kernel // 2 # Calcula o tamanho do padding (borda) necessário ao redor da imagem
        saida = np.zeros_like(imagem_float) 
        imagem_com_padding = np.pad(imagem_float, ((pad_alt, pad_alt), (pad_larg, pad_larg)), mode='constant', constant_values=0) # Adiciona uma borda de zeros ao redor da imagem
        for y in range(alt_img): # - y percorre as linhas 0 a alt_img
            for x in range(larg_img): # - x percorre as colunas 0 a larg_img
                saida[y, x] = np.sum(kernel * imagem_com_padding[y:y+alt_kernel, x:x+larg_kernel]) # Calcula o valor do pixel na posição (y, x) na imagem de saída
        return saida # Retorna a imagem convoluída

    # Gera kernel gaussiano 2D separável ( Suavização )
    def _obter_kernel_gaussiano_2d(self, tamanho_kernel, sigma):
        kernel_1d = np.zeros(tamanho_kernel); centro = tamanho_kernel//2 # Cria um array 1D 
        for i in range(tamanho_kernel): x=i-centro; kernel_1d[i]=(1/(np.sqrt(2*np.pi)*sigma))*np.exp(-(x**2)/(2*sigma**2)) # Preenche o kernel 1D com valores da função gaussiana
        kernel_1d /= np.sum(kernel_1d) # Normaliza o kernel para que a soma de todos os valores seja 1
        return np.outer(kernel_1d, kernel_1d)# Cria o kernel 2D a partir do kernel 1D usando o produto externo
    
    # -------------------------------------------- AJUSTES -------------------------------------------------------------
    
    # Negativo: pega o Inverso da cor
    def _processar_negativo(self, imagem): return 255 - imagem

    # Ajusta brilho e contraste no espaço HSI, modificando apenas a intensidade (I)
    def _processar_brilho_contraste_hsi(self, imagem, brilho, contraste):
        if len(imagem.shape) < 3: # Verifica se a imagem tem menos de 3 dimensões
            img_float = imagem.astype(np.float64); img_float = img_float * contraste + (brilho*128); return np.clip(img_float,0,255).astype(np.uint8) # cinza
        imagem_rgb = imagem[..., ::-1].copy(); imagem_hsi = self._rgb_para_hsi(imagem_rgb) # Trata imagens coloridas
        I = imagem_hsi[..., 2]; I = I * contraste + brilho; imagem_hsi[..., 2] = np.clip(I, 0, 1) # Ajusta a intensidade (I) no espaço HSI
        rgb_final = self._hsi_para_rgb(imagem_hsi) # Converte a imagem HSI ajustada de volta para RGB
        return rgb_final[..., ::-1].copy() # Retorna a imagem final

    # Aplica correção de gamma
    def _processar_gamma(self, imagem, gamma):
        if gamma<=0: gamma=0.01 # Verifica se o valor de `gamma` é menor ou igual a 0
        lut = np.array([((i/255.0)**(1.0/gamma))*255 for i in np.arange(256)]).astype("uint8")
        return self._aplicar_lut(imagem, lut) # Aplica a tabela de lookup à imagem
        
    # Equaliza o histograma de um canal único usando função de distribuição cumulativa
    def _equalizar_canal_unico(self, canal):
        hist, _ = np.histogram(canal.flatten(),256,[0,256]); cdf = hist.cumsum(); cdf_m = np.ma.masked_equal(cdf,0) 
        if cdf_m.max() == cdf_m.min(): return canal # Verifica se todos os valores da CDF são iguais
        cdf_m = (cdf_m-cdf_m.min())*255/(cdf_m.max()-cdf_m.min()); cdf_final = np.ma.filled(cdf_m,0).astype('uint8') # Converte a CDF mascarada de volta para um array normal
        return cdf_final[canal] # Aplica a equalização ao canal original

    def _processar_equalizacao_hist(self, imagem):
        if len(imagem.shape) > 2: # Trata imagens coloridas
            imagem_rgb = imagem[..., ::-1].copy(); hsi = self._rgb_para_hsi(imagem_rgb) # Cria uma cópia para não alterar a original
            I_canal = (hsi[:,:,2]*255).astype(np.uint8); I_equalizado = self._equalizar_canal_unico(I_canal) # Aplica a equalização apenas ao canal de intensidade
            hsi[:,:,2] = I_equalizado.astype(np.float32)/255.0 # Substitui o canal I na imagem HSI pelo valor equalizado
            rgb_final = self._hsi_para_rgb(hsi) # Converte a imagem HSI de volta para RGB
            return rgb_final[..., ::-1].copy() # Inverte os canais de volta e retorna uma cópia 
        else: return self._equalizar_canal_unico(imagem) # Trata imagens em escala de cinza

    # ------------------------------------------------- REDIMENSIONAR -----------------------------------------------------
    
    # Redimensiona imagem usando interpolação por vizinho mais próximo     
    def _processar_redimensionamento_vizinho(self, imagem, fator):
        alt_orig, larg_orig, canais = imagem.shape
        nova_alt, nova_larg = int(round(alt_orig*fator)), int(round(larg_orig*fator))
        if nova_alt<=0 or nova_larg<=0: return self.imagem_antes_do_filtro
        nova_imagem=np.zeros((nova_alt,nova_larg,canais), dtype=np.uint8)
        escala_alt, escala_larg = float(alt_orig)/nova_alt, float(larg_orig)/nova_larg
        for y in range(nova_alt):
            for x in range(nova_larg): # Loop sobre cada pixel da nova imagem
                orig_y=min(int(round(y*escala_alt)),alt_orig-1); orig_x=min(int(round(x*escala_larg)),larg_orig-1) # Mapeia as coordenadas da nova imagem para a original
                nova_imagem[y, x]=imagem[orig_y, orig_x] # Copia o valor do pixel da posição original para a nova imagem
        return nova_imagem

    # Redimensiona imagem usando interpolação bilinear 
    def _processar_redimensionamento_bilinear(self, imagem, fator):
        alt_orig, larg_orig, canais = imagem.shape
        nova_alt, nova_larg = int(round(alt_orig*fator)), int(round(larg_orig*fator))
        if nova_alt<=0 or nova_larg<=0: return imagem
        x_novo, y_novo = np.meshgrid(np.arange(nova_larg), np.arange(nova_alt)) # Cria grades de coordenadas para a nova imagem
        escala_x, escala_y = larg_orig/nova_larg, alt_orig/nova_alt # Calcula as escalas de redimensionamento
        x_orig, y_orig = x_novo*escala_x, y_novo*escala_y # Mapeia as coordenadas da nova imagem para a original
        x1, y1 = x_orig.astype(int), y_orig.astype(int) # Pega as coordenadas inteiras inferiores
        x2, y2 = np.clip(x1+1,0,larg_orig-1), np.clip(y1+1,0,alt_orig-1) # Pega as coordenadas inteiras superiores
        dx, dy = x_orig-x1, y_orig-y1 # Calcula as frações decimais (distâncias) entre as coordenadas
        q11, q21, q12, q22 = imagem[y1,x1], imagem[y1,x2], imagem[y2,x1], imagem[y2,x2] # Pega os valores dos 4 pixels vizinhos
        b1 = q11*(1-dx[...,np.newaxis])+q21*dx[...,np.newaxis] # Interpola horizontalmente
        b2 = q12*(1-dx[...,np.newaxis])+q22*dx[...,np.newaxis]
        pixel_final = b1*(1-dy[...,np.newaxis])+b2*dy[...,np.newaxis] # Interpola verticalmente
        return np.clip(pixel_final,0,255).astype(np.uint8) # Finaliza a imagem
        
    # -------------------------------------------- FILTROS ESPACIAIS -----------------------------------------------------------
    # FILTROS DE SUAVIZAÇÃO

    # Aplica filtro de caixa (média) usando convolução 2D
    def _processar_filtro_caixa(self, imagem, tamanho_kernel): 
        kernel = np.ones((tamanho_kernel,tamanho_kernel), np.float64)/(tamanho_kernel**2) # Cria o kernel de caixa
        resultado=np.zeros_like(imagem, dtype=np.float64) # Cria uma array com o mesmo formato da imagem (altura, largura, canais)
        for c in range(imagem.shape[2]): resultado[...,c]=self._convolucao_manual_2d(imagem[...,c],kernel) # Aplica a convolução para cada canal da imagem
        return np.clip(resultado,0,255).astype(np.uint8) # Finaliza a imagem

    # Aplica filtro gaussiano usando convolução 2D com kernel gerado
    def _processar_filtro_gaussiano(self, imagem, tamanho_kernel, sigma): 
        if sigma<=0: sigma=0.1 # Verifica se `sigma` (desvio padrão) é válido
        kernel=self._obter_kernel_gaussiano_2d(tamanho_kernel,sigma) # Gera o kernel gaussiano
        resultado=np.zeros_like(imagem,dtype=np.float64) # Cria uma array com o mesmo formato da imagem usando float64
        for c in range(imagem.shape[2]): resultado[...,c]=self._convolucao_manual_2d(imagem[...,c],kernel) # Aplica a convolução para cada canal
        return np.clip(resultado,0,255).astype(np.uint8)

    # FILTRO DE AGUÇAMENTO

    # Aplica aguçamento usando o operador Laplaciano. Subtrai o Laplaciano ponderado para realçar borda
    def _processar_agucamento_laplaciano(self, imagem, fator):
        imagem_float=imagem.astype(np.float64)
        kernel=np.array([[0,1,0],[1,-4,1],[0,1,0]],dtype=np.float64) # Define o kernel Laplaciano 3x3
        resultado=np.zeros_like(imagem_float) ## Cria uma array `resultado` com o mesmo formato da imagem (altura, largura, canais)
        for c in range(imagem.shape[2]):
            laplaciano=self._convolucao_manual_2d(imagem_float[...,c],kernel)
            resultado[...,c]=imagem_float[...,c]-fator*laplaciano # Aplica a convolução e aguçamento por canal
        return np.clip(resultado,0,255).astype(np.uint8)

    # Aplica aguçamento usando o gradiente de Sobel. Adiciona a magnitude do gradiente para realçar bordas
    def _processar_agucamento_gradiente(self, imagem, fator):
        imagem_float=imagem.astype(np.float64)
        kernel_sobel_x=np.array([[-1,0,1],[-2,0,2],[-1,0,1]],dtype=np.float64)
        kernel_sobel_y=np.array([[-1,-2,-1],[0,0,0],[1,2,1]],dtype=np.float64) # Define os kernels de Sobel
        cinza=self._para_cinza_numpy(imagem).astype(np.float64) # Converte a imagem RGB para escala de cinza
        grad_x=self._convolucao_manual_2d(cinza,kernel_sobel_x); grad_y=self._convolucao_manual_2d(cinza,kernel_sobel_y) # Calcula os gradientes
        magnitude=np.sqrt(grad_x**2+grad_y**2) # Calcula a magnitude do gradiente
        resultado=np.zeros_like(imagem_float)
        for c in range(imagem.shape[2]): resultado[...,c]=imagem_float[...,c]+fator*magnitude # Aplica o aguçamento
        return np.clip(resultado,0,255).astype(np.uint8)

    # --------------------------------------------------- FREQUÊNCIA ------------------------------------------------------

    # Cria filtro no domínio da frequência  Butterworth. Passa-baixa por padrão, passa-alta para 'agucamento
    def _criar_filtro_frequencia(self, forma, d0, tipo, n):
        P, Q = forma; H=np.zeros((P,Q),np.float32) # Desempacota as dimensões da imagem
        for u in range(P):
            for v in range(Q):
                D_uv=np.sqrt((u-P/2)**2+(v-Q/2)**2) # Loop sobre cada ponto (u, v) no domínio da frequência
                if d0 > 0: H[u,v]=1/(1+(D_uv/d0)**(2*n)) # Aplica o filtro Butterworth (passa-baixa) se `d0 > 0`
        if tipo=='agucamento':return 1-H # Define o tipo de filtro
        return H # suavização

    # Indica que o método usa Transformada de Fourier para processar a imagem
    def _processar_filtro_frequencia(self, imagem, tipo, d0, n=2):
        imagem_cinza=self._para_cinza_numpy(imagem); M,N=imagem_cinza.shape; P,Q=2*M,2*N # Converte a imagem para escala de cinza
        fp=np.zeros((P,Q),np.float32); fp[0:M,0:N]=imagem_cinza #  Matriz de zeros do tamanho expandido
        for x in range(P):
            for y in range(Q): fp[x,y]*=(-1)**(x+y) # Centraliza o espectro
        F=np.fft.fft2(fp) # Calcula a FFT 2D, convertendo a imagem para o domínio da frequência
        H=self._criar_filtro_frequencia((P,Q),d0,tipo,n) # Gera o filtro com as dimensões expandidas
        G=H*F; gp=np.fft.ifft2(G).real # Aplica o filtro multiplicando elemento por elemento.
        for x in range(P):
            for y in range(Q): gp[x,y]*=(-1)**(x+y) # Reverte a centralização.
        resultado=gp[0:M,0:N] # Recorta para o tamanho original.
        min_val,max_val=np.min(resultado),np.max(resultado)
        if max_val>min_val: resultado=(resultado-min_val)/(max_val-min_val)*255 # Normaliza para [0, 255] se houver variação.
        resultado=np.clip(resultado,0,255).astype(np.uint8) # Limita e converte para uint8.
        if len(imagem.shape)>2: return np.stack([resultado]*3,axis=-1) 
        return resultado # Adiciona canais para RGB ou retorna como cinza.
    
    # --------------------------------------------------- MORFOLOGIA -------------------------------------------------------------------

    # Aplica limiarização para criar imagem binária
    def _processar_limiarizacao(self, imagem, limiar):
        imagem_cinza = self._para_cinza_numpy(imagem) # Converte para escala de cinza.
        imagem_binaria = np.where(imagem_cinza > limiar, 255, 0).astype(np.uint8) # Se `imagem_cinza > limiar`, usa 255; senão, 0.
        return imagem_binaria # Retorna a imagem binária.

    # Aplica erosão ou dilatação morfológica. Usa kernel quadrado para modificar formas.
    def _processar_morfologia(self, imagem, operacao, k):
        imagem_cinza = self._para_cinza_numpy(imagem) # - Converte para cinza
        tamanho_kernel = k
        kernel = np.ones((tamanho_kernel, tamanho_kernel), np.uint8)
        alt_img, larg_img = imagem_cinza.shape
        pad = tamanho_kernel // 2 
        
        saida = np.zeros_like(imagem_cinza) # Cria array de saída com zeros.
        
        # Para erosão, o padding ideal é com valor alto; para dilatação, com valor baixo.
        val_pad = 255 if operacao == 'erosao' else 0
        img_pad = np.pad(imagem_cinza, pad, mode='constant', constant_values=val_pad) # 255 para erosão (preserva brancos), 0 para dilatação

        # Loop sobre cada pixel
        for y in range(alt_img):
            for x in range(larg_img):
                vizinhanca = img_pad[y:y+tamanho_kernel, x:x+tamanho_kernel]
                if operacao == 'erosao':
                    saida[y, x] = np.min(vizinhanca)
                elif operacao == 'dilatacao':
                    saida[y, x] = np.max(vizinhanca)
        return saida # Retorna a imagem processada

    # Aplica uma função de processamento e atualiza a interface
    def aplicar_acao_direta(self, func_proc):
        if not self._verificar_imagem_carregada(): return
        self.adicionar_ao_historico()
        self.imagem_processada=func_proc(self.imagem_processada)
        self.exibir_imagem(self.imagem_processada)

    def aplicar_negativo(self): self.aplicar_acao_direta(self._processar_negativo)
    def aplicar_equalizacao_hist(self): self.aplicar_acao_direta(self._processar_equalizacao_hist)
    def visualizar_espectro_fourier(self): self.aplicar_acao_direta(self._visualizar_espectro_fourier_proc)

    # Visualiza o espectro de Fourier da imagem
    def _visualizar_espectro_fourier_proc(self, imagem):
        imagem_cinza = self._para_cinza_numpy(imagem)
        f = np.fft.fft2(imagem_cinza); fshift = np.fft.fftshift(f) # Calcula FFT e centraliza
        magnitude_espectro = 20*np.log(np.abs(fshift) + 1) # Correção para escala logarítmica
        espectro_visual = np.zeros_like(magnitude_espectro, dtype=np.uint8)
        min_val, max_val = np.min(magnitude_espectro), np.max(magnitude_espectro) # Converte magnitude para escala logarítmica para visualização
        if max_val > min_val:
            espectro_visual = (255 * (magnitude_espectro - min_val) / (max_val - min_val)).astype(np.uint8) # Normaliza para [0, 255] e converte para uint8
        return espectro_visual
    
    # -------------------------------------------------------------- CONTROLES ------------------------------------------------------------------

    def mostrar_controles_brilho_contraste_hsi(self): self.construir_controles({'window_title':'Brilho e Contraste (HSI)','process_function':self._processar_brilho_contraste_hsi,'params':[{'id':'brilho','name':'Brilho','min':-100,'max':100,'initial':0,'type':'float','divisor':100.0},{'id':'contraste','name':'Contraste','min':0,'max':300,'initial':100,'type':'float','divisor':100.0}]})
    def mostrar_controles_gamma(self): self.construir_controles({'window_title':'Correção Gamma','process_function':self._processar_gamma,'params':[{'id':'gamma','name':'Gamma','min':10,'max':250,'initial':100,'type':'float','divisor':100.0}]})
    def mostrar_controles_redim_vizinho(self): self.construir_controles({'window_title':'Redimensionar (Vizinho Mais Próximo)','process_function':self._processar_redimensionamento_vizinho,'params':[{'id':'fator','name':'Fator','min':10,'max':500,'initial':100,'type':'float','divisor':100.0}]})
    def mostrar_controles_redim_bilinear(self): self.construir_controles({'window_title':'Redimensionar (Bilinear)','process_function':self._processar_redimensionamento_bilinear,'params':[{'id':'fator','name':'Fator','min':10,'max':500,'initial':100,'type':'float','divisor':100.0}]})
    def mostrar_controles_filtro_caixa(self): self.construir_controles({'window_title':'Filtro de Caixa','process_function':self._processar_filtro_caixa,'params':[{'id':'tamanho_kernel','name':'Kernel','min':3,'max':99,'initial':3,'odd':True,'step':2}]})
    def mostrar_controles_gaussiano(self): self.construir_controles({'window_title':'Filtro Gaussiano','process_function':self._processar_filtro_gaussiano,'params':[{'id':'tamanho_kernel','name':'Kernel','min':3,'max':99,'initial':3,'odd':True,'step':2},{'id':'sigma','name':'Sigma','min':1,'max':500,'initial':10,'type':'float','divisor':10.0}]})
    def mostrar_controles_agucar_laplaciano(self): self.construir_controles({'window_title':'Aguçar (Laplaciano)','process_function':self._processar_agucamento_laplaciano,'params':[{'id':'fator','name':'Intensidade','min':1,'max':100,'initial':10,'type':'float','divisor':10.0}]})
    def mostrar_controles_agucar_gradiente(self): self.construir_controles({'window_title':'Aguçar (Gradiente)','process_function':self._processar_agucamento_gradiente,'params':[{'id':'fator','name':'Intensidade','min':1,'max':50,'initial':4,'type':'float','divisor':10.0}]})
    def mostrar_controles_suavizacao_freq(self): self.construir_controles({'window_title':'Suavização (Frequência)','process_function':lambda img,d0,n:self._processar_filtro_frequencia(img,'suavizacao',d0,n),'params':[{'id':'d0','name':'Freq. Corte D0','min':1,'max':200,'initial':30},{'id':'n','name':'Ordem (n)','min':1,'max':10,'initial':2}]})
    def mostrar_controles_agucamento_freq(self): self.construir_controles({'window_title':'Aguçamento (Frequência)','process_function':lambda img,d0,n:self._processar_filtro_frequencia(img,'agucamento',d0,n),'params':[{'id':'d0','name':'Freq. Corte D0','min':1,'max':200,'initial':30},{'id':'n','name':'Ordem (n)','min':1,'max':10,'initial':2}]})
    def mostrar_controles_limiar(self): self.construir_controles({'window_title':'Limiarização','process_function':self._processar_limiarizacao,'params':[{'id':'limiar','name':'Limiar','min':0,'max':255,'initial':127}]})
    def mostrar_controles_erosao(self): self.construir_controles({'window_title':'Erosão Morfológica','process_function':lambda img,k:self._processar_morfologia(img,'erosao',k),'params':[{'id':'k','name':'Kernel','min':3,'max':51,'initial':3,'odd':True,'step':2}]})
    def mostrar_controles_dilatacao(self): self.construir_controles({'window_title':'Dilatação Morfológica','process_function':lambda img,k:self._processar_morfologia(img,'dilatacao',k),'params':[{'id':'k','name':'Kernel','min':3,'max':51,'initial':3,'odd':True,'step':2}]})

   # Abre uma imagem do sistema de arquivos     
    def abrir_imagem(self):
        nome_arquivo, _ = QFileDialog.getOpenFileName(self, 'Abrir Imagem', '', 'Imagens (*.png *.jpg *.jpeg *.bmp)')
        if nome_arquivo:
            self.imagem_original = cv2.imread(nome_arquivo)
            if self.imagem_original is None: QMessageBox.critical(self,"Erro",f"Não foi possível ler a imagem: {nome_arquivo}"); return
            self.imagem_processada = self.imagem_original.copy()
            self.pilha_historico.clear(); self.pilha_refazer.clear(); self.atualizar_botoes_historico()
            self.rotulo_imagem.setStyleSheet("")
            self.exibir_imagem(self.imagem_processada)

    # Salva a imagem processada    
    def salvar_imagem(self):
        if not self._verificar_imagem_carregada(): return
        nome_arquivo, _ = QFileDialog.getSaveFileName(self, 'Salvar Imagem', '', 'PNG (*.png);;JPEG (*.jpg)')
        if nome_arquivo: 
            # Para salvar, precisamos converter de volta para o padrão BGR do OpenCV
            imagem_salvar = self.imagem_processada[..., ::-1].copy() if len(self.imagem_processada.shape) == 3 else self.imagem_processada
            cv2.imwrite(nome_arquivo, imagem_salvar)

    # Restaura a imagem original     
    def reverter_para_original(self):
        if not self._verificar_imagem_carregada(): return
        self.adicionar_ao_historico()
        self.imagem_processada = self.imagem_original.copy()
        self.exibir_imagem(self.imagem_processada)

    # Exibe a imagem na interface GUI    
    def exibir_imagem(self, imagem):
        if imagem is None: return
        
        if len(imagem.shape) == 3 and imagem.shape[2] == 3:
            alt, larg, _ = imagem.shape; bytes_por_linha = 3*larg
            imagem_rgb = imagem[..., ::-1].copy()
            q_img = QImage(imagem_rgb.data, larg, alt, bytes_por_linha, QImage.Format_RGB888)
        else:
            alt, larg = imagem.shape; bytes_por_linha = larg
            q_img = QImage(imagem.data, larg, alt, bytes_por_linha, QImage.Format_Grayscale8)
            
        self.rotulo_imagem.setPixmap(QPixmap.fromImage(q_img))
        self.rotulo_imagem.adjustSize()

    # Limpa um layout da GUI   
    def limpar_layout(self, layout):
        while layout.count():
            filho = layout.takeAt(0)
            if filho.widget(): filho.widget().deleteLater()
            elif filho.layout(): self.limpar_layout(filho.layout())

# Inicia a aplicação
def main():
    aplicacao = QApplication(sys.argv)
    aplicacao.setStyle('Fusion')
    aplicacao.setStyleSheet(estilo_escuro)
    janela = EditorDeImagensApp()
    janela.show()
    sys.exit(aplicacao.exec_())

if __name__ == "__main__":
    main()