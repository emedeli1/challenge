// Pruebas del ejercicio 9: mine::Pixels
#include <type_traits>
#include <ejercicios/ej09.hpp>
#include <ejercicios/pruebas/prueba.hpp>

using mine::Pixels;

// Can Pixels be added to a float, or multiplied by Pixels? Neither must compile: these
// say if they would
template <typename T>
concept AddsAFloat = requires(T p) { p + 1.0f; };
template <typename T>
concept MultipliesItself = requires(T p) { p * p; };

int main() {
   Prueba t{ "Ejercicio 9: Pixels, unos píxeles que no se mezclan con nada" };

   t.p(1, "lo normal");
   t.expect_near("6,5 + 12 px", (Pixels{ 6.5f } + Pixels{ 12.0f }).value(), 18.5f);
   t.expect("20 - 5 == 15", Pixels{ 20.0f } - Pixels{ 5.0f } == Pixels{ 15.0f }, true);
   t.expect("200 px * 0,5 == 100 px", Pixels{ 200.0f } * 0.5f == Pixels{ 100.0f }, true);
   t.expect("3 < 4", Pixels{ 3.0f } < Pixels{ 4.0f }, true);
   t.expect("12 >= 12", Pixels{ 12.0f } >= Pixels{ 12.0f }, true);
   t.expect("8 != 9", Pixels{ 8.0f } != Pixels{ 9.0f }, true);

   t.p(2, "los límites");
   t.expect("Pixels{} es 0", Pixels{} == Pixels{ 0.0f }, true);
   t.expect("un float no se convierte solo en Pixels (explicit)"
          , std::is_convertible_v<float, Pixels>, false);
   t.expect("no se puede sumar un float a unos Pixels", AddsAFloat<Pixels>, false);
   t.expect("no se pueden multiplicar Pixels por Pixels", MultipliesItself<Pixels>
          , false);
   t.expect("ocupa lo mismo que un float", sizeof(Pixels), sizeof(float));
   constexpr Pixels in_compilation{ Pixels{ 2.0f } * 3.0f - Pixels{ 1.0f } };
   t.expect_near("se calcula al compilar (constexpr)", in_compilation.value(), 5.0f);
}
