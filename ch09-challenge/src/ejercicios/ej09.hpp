#pragma once
//========================================================================================
// Ejercicio 9: Pixels, unos píxeles que no se mezclan con nada
//
// TIENES QUE COMPLETAR ALGO: el tipo mine::Pixels (su dato, n_, ya está), en
// ❂❂ Your code ❂❂.
//
// 1. Pasa aquí tu código del papel, tal cual.
// 2. Ejecuta `make ej09`: las pruebas P1 y P2.
// 3. Si algo falla, corrígelo y explica cada cambio con su comentario:
//    `// FIX <tipo><nº>: <motivo>` (N, E, S o F, como dice la rúbrica).
//    Hasta que escribas todo lo que pide el enunciado, las pruebas no compilan: el
//    compilador dice qué falta.
//
// Es un tipo de cabecera (todo constexpr): va en el espacio de nombres `mine` y el
// juego no lo usa.
//========================================================================================
#include <compare>

namespace mine {

// A distance in pixels. Not a float: a float is not Pixels by accident, and Pixels are
// only added to Pixels
struct Pixels {
   // ❂❂ Your code ❂❂

private:
   float n_{};
};

} // namespace mine
