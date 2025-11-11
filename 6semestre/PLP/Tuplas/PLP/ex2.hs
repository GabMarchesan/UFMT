triangulo :: (Float, Float, Float) -> (String, Float)
triangulo (a, b, c)
    | a + b > c && a + c > b && b + c > a = (tipoTriangulo, perimetro)
    | otherwise = ("Nao e um triangulo", 0)
    where
        perimetro = a + b + c
        tipoTriangulo
            | a == b && b == c = "Equilatero"
            | a == b || a == c || b == c = "Isosceles"
            | otherwise = "Escaleno"

