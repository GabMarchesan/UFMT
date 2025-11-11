USE Clinica;

-- Inserções em Recepcionista
INSERT INTO Recepcionista (CPF_RECEP, TELEFONE, NOME) VALUES
('44444444444', '11955556666', 'Mariana Albuquerque'),
('55555555555', '11966667777', 'Rafael Pereira'),
('66666666666', '11977778888', 'Juliana Souza');

-- Inserções em Medico
INSERT INTO Medico (CRM, ESPEC, TEL, NOME) VALUES
('CRM004', 'Neurologia', '1133333001', 'Dr. André Costa'),
('CRM005', 'Dermatologia', '1133333002', 'Dra. Patricia Lima'),
('CRM006', 'Oftalmologia', '1133333003', 'Dr. Eduardo Silva');

-- Inserções em ConvenioMedico
INSERT INTO ConvenioMedico (CODIGO_CONV, NOME, PORCENTAGEM_COBERTURA) VALUES
(4, 'Bradesco Saúde', 85.00),
(5, 'Porto Seguro', 75.00),
(6, 'Golden Cross', 65.00);

-- Inserções em Paciente
INSERT INTO Paciente (CPF_PACIENTE, CODIGO_CONVENIO, DT_NASC, TELEFONE, NOME, ENDERECO) VALUES
('77788899900', 4, '1988-06-20', '11944443333', 'Beatriz Almeida', 'Rua E, 500'),
('88899900011', 5, '1992-09-15', '11955554444', 'Gustavo Ribeiro', 'Av. F, 600'),
('99900011122', NULL, '1983-04-12', '11966665555', 'Larissa Oliveira', 'Rua G, 700'),
('00011122233', 6, '1997-11-25', '11977776666', 'Thiago Pereira', 'Rua H, 800');

-- Inserções em Consulta
INSERT INTO Consulta (CPF_RECEP, CPF_PACIENTE, CRM_MED, HORA, DATA, STATUS) VALUES
('44444444444', '77788899900', 'CRM004', '09:30:00', '2025-05-10', 'agendada'),
('44444444444', '88899900011', 'CRM005', '13:00:00', '2025-05-10', 'concluida'),
('55555555555', '77788899900', 'CRM004', '10:00:00', '2025-04-15', 'concluida'),
('55555555555', '99900011122', 'CRM004', '14:30:00', '2025-05-01', 'concluida'),
('66666666666', '00011122233', 'CRM006', '16:00:00', '2025-05-15', 'agendada'),
('55555555555', '88899900011', 'CRM005', '11:30:00', '2025-05-05', 'agendada');

-- Inserções em ProcedimentoMedico
INSERT INTO ProcedimentoMedico (CODIGO_PRO, TEMPO, VALOR, NOME) VALUES
(201, '00:40:00', 250.00, 'Tomografia'),
(202, '00:25:00', 180.00, 'Avaliação Inicial'),
(203, '01:10:00', 350.00, 'Exame de Imagem'),
(204, '00:50:00', 280.00, 'Ultrassom');

-- Inserções em GeraProcedimento
INSERT INTO GeraProcedimento (ID_CONSULTA, CODIGO_PRO) VALUES
(1, 202),
(2, 202),
(3, 201),
(4, 203),
(4, 204),
(5, 202);

-- Inserções em CoberturaConvenio
INSERT INTO CoberturaConvenio (CODIGO_CONV, CODIGO_PRO) VALUES
(4, 201),
(4, 202),
(5, 202),
(6, 203),
(6, 204);

-- Inserções em Pagamento
INSERT INTO Pagamento (CODIGO_CONV, CPF_PACIENTE, ID_CONSULTA, VALOR, FORMA, STATUS, DATA) VALUES
(NULL, '77788899900', 1, 180.00, 'cartao', 'pago', '2025-05-05'),
(5, NULL, 2, 180.00, 'dinheiro', 'pago', '2025-05-06'),
(4, NULL, 3, 250.00, 'boleto', 'pago', '2025-04-15'),
(NULL, '99900011122', 4, 350.00, 'cartao', 'pendente', NULL),
(6, NULL, 5, 180.00, 'boleto', 'pago', '2025-05-10');