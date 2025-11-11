type Pessoa = (String, Int, Float, Char)

pess :: Int -> Pessoa
pess x
    | x == 1 = ("Rosa", 27, 1.66,'F')
    | x == 2 = ("João", 26, 1.85,'M')
    | x == 3 = ("Maria", 67, 1.55,'F')
    | x == 4 = ("Jose", 48, 1.78, 'M')
    | x == 5 = ("Paulo", 24, 1.93, 'M')
    | x == 6 = ("Clara", 38, 1.70,'F')
    | x == 7 = ("Bob", 12, 1.45, 'M')
    | x == 8 = ("Rosana", 31, 1.58,'F')
    | x == 9 = ("Daniel", 75, 1.74, 'M')
    | x == 10 = ("Jocileide", 21, 1.69,'F')
    | otherwise = ("Acabou!", 0, 0.0, 'x')

maiorIdade :: Int -> Int
maiorIdade x
    | x == 1 = 1
    | otherwise = maior x (maiorIdade (x-1))
    where
        maior x y
            | idade (pess x) >= idade (pess y) = x
            | otherwise = y

idade :: Pessoa -> Int
idade (_, x, _, _) = x

idadeMedia :: Float
idadeMedia = fromIntegral (somaIdades 10) / 10
    where
        somaIdades 0 = 0
        somaIdades n = idade (pess n) + somaIdades (n-1)

mascSup25 :: Int -> Int
mascSup25 n = fromIntegral $ length [x | x <- [1..n], sexo (pess x) == 'M', idade (pess x) > 25]

sexo :: Pessoa -> Char
sexo (_, _, _, s) = s
