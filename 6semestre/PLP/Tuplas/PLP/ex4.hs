serie :: (Float, Int) -> Float
serie (x, n) = serieAux (x, fromIntegral n, 1, 0)

serieAux :: (Float, Float, Float, Float) -> Float
serieAux (_, 0, _, result) = result
serieAux (x, n, term, result) =
    serieAux (x, n-1, nextTerm, result + currentTerm)
    where
        currentTerm = if even (truncate term)
                      then x / term
                      else term / x
        nextTerm = term + 1

