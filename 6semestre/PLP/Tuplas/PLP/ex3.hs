somaP :: Int -> Int
somaP n = somaPImpares n 0
    where
        somaPImpares :: Int -> Int -> Int
        somaPImpares 0 acc = acc
        somaPImpares num acc
            | odd num = somaPImpares (num - 1) (acc + num)
            | otherwise = somaPImpares (num - 1) acc

