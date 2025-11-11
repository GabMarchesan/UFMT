USE Clinica;

-- 1. Listar todas as consultas agendadas para um determinado médico em um dia específico
SELECT c.ID_CONSULTA, c.DATA, c.HORA, p.NOME AS PACIENTE
FROM Consulta c
JOIN Paciente p ON c.CPF_PACIENTE = p.CPF_PACIENTE
WHERE c.CRM_MED = 'CRM004'
  AND c.DATA = '2025-05-10'
  AND c.STATUS = 'agendada';

-- 2. Contar quantas consultas cada médico realizou no último mês
SELECT c.CRM_MED, m.NOME, COUNT(*) AS TOTAL_CONSULTAS
FROM Consulta c
JOIN Medico m ON c.CRM_MED = m.CRM
WHERE c.STATUS = 'concluida'
  AND c.DATA BETWEEN '2025-04-01' AND '2025-05-01'
GROUP BY c.CRM_MED, m.NOME;

-- 3. Encontrar os pacientes que nunca tiveram uma consulta agendada
SELECT p.CPF_PACIENTE, p.NOME
FROM Paciente p
LEFT JOIN Consulta c ON p.CPF_PACIENTE = c.CPF_PACIENTE
WHERE c.ID_CONSULTA IS NULL;

-- 4. Listar os procedimentos realizados em uma consulta específica
SELECT pm.CODIGO_PRO, pm.NOME, pm.VALOR, pm.TEMPO
FROM GeraProcedimento gp
JOIN ProcedimentoMedico pm ON gp.CODIGO_PRO = pm.CODIGO_PRO
WHERE gp.ID_CONSULTA = 4;

-- 5. Calcular o total pago por cada paciente nos últimos 3 meses
SELECT p.CPF_PACIENTE, pac.NOME, SUM(p.VALOR) AS TOTAL_PAGO
FROM Pagamento p
JOIN Paciente pac ON p.CPF_PACIENTE = pac.CPF_PACIENTE
WHERE p.STATUS = 'pago'
  AND p.DATA BETWEEN '2025-02-01' AND '2025-05-01'
  AND p.CPF_PACIENTE IS NOT NULL
GROUP BY p.CPF_PACIENTE, pac.NOME;

-- 6. Identificar os convênios que cobrem mais de 50% do valor das consultas
SELECT CODIGO_CONV, NOME, PORCENTAGEM_COBERTURA
FROM ConvenioMedico
WHERE PORCENTAGEM_COBERTURA > 50.0;

-- 7. Listar todos os pacientes atendidos por um determinado médico
SELECT DISTINCT p.CPF_PACIENTE, p.NOME
FROM Consulta c
JOIN Paciente p ON c.CPF_PACIENTE = p.CPF_PACIENTE
WHERE c.CRM_MED = 'CRM004';

-- 8. Mostrar o faturamento total do último mês, considerando apenas pagamentos concluídos
SELECT SUM(VALOR) AS FATURAMENTO_TOTAL
FROM Pagamento
WHERE STATUS = 'pago'
  AND DATA BETWEEN '2025-04-01' AND '2025-05-01';

-- 9. Encontrar os médicos que possuem mais de 5 consultas marcadas para a próxima semana
SELECT c.CRM_MED, m.NOME, COUNT(*) AS TOTAL_CONSULTAS
FROM Consulta c
JOIN Medico m ON c.CRM_MED = m.CRM
WHERE c.STATUS = 'agendada'
  AND c.DATA BETWEEN '2025-05-01' AND '2025-05-08'
GROUP BY c.CRM_MED, m.NOME
HAVING COUNT(*) > 5;

-- 10. Listar os pacientes que mais gastaram na clínica nos últimos 6 meses
SELECT p.CPF_PACIENTE, pac.NOME, SUM(p.VALOR) AS TOTAL_GASTO
FROM Pagamento p
JOIN Paciente pac ON p.CPF_PACIENTE = pac.CPF_PACIENTE
WHERE p.STATUS = 'pago'
  AND p.DATA BETWEEN '2024-11-01' AND '2025-05-01'
  AND p.CPF_PACIENTE IS NOT NULL
GROUP BY p.CPF_PACIENTE, pac.NOME
ORDER BY TOTAL_GASTO DESC
LIMIT 10;