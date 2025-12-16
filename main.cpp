#include <vector>
#include <emscripten.h>

// --------------------------------------------------------------------------
// パラメータ
// --------------------------------------------------------------------------
const int N = 100;
const double L = 1.0;
const double dx = L / (N - 1);
const double alpha = 0.01;

// --------------------------------------------------------------------------
// シミュレーション配列（JSと共有）
// --------------------------------------------------------------------------
std::vector<double> T(N, 0.0);        // ★ JS が見る配列
std::vector<double> T_next(N, 0.0);   // 作業用

// --------------------------------------------------------------------------
// 初期化
// --------------------------------------------------------------------------
void initialize_simulation() {
    for (int i = 0; i < N; ++i) {
        T[i] = 0.0;
        T_next[i] = 0.0;
    }

    // 中央に 100 のピーク
    for (int i = N / 2 - 10; i <= N / 2 + 10; ++i) {
        T[i] = 100.0;
    }
}

// --------------------------------------------------------------------------
// JS から呼ばれる関数
// --------------------------------------------------------------------------
extern "C" {

EMSCRIPTEN_KEEPALIVE
void initialize_simulation_c() {
    initialize_simulation();
}

EMSCRIPTEN_KEEPALIVE
void one_step_and_draw(double dt) {

    double r = alpha * dt / (dx * dx);

    for (int i = 1; i < N - 1; ++i) {
        T_next[i] = T[i] + r * (T[i + 1] - 2.0 * T[i] + T[i - 1]);
    }

    // 境界条件
    T_next[0] = 0.0;
    T_next[N - 1] = 0.0;

    // ★★★ JS と共有しているため swap しない ★★★
    for (int i = 0; i < N; ++i) {
        T[i] = T_next[i];
    }
}

EMSCRIPTEN_KEEPALIVE
int get_array_size() {
    return N;
}

EMSCRIPTEN_KEEPALIVE
double* get_temperature_data() {
    return T.data();   // ★ このポインタは一生変えない
}

} // extern "C"

// --------------------------------------------------------------------------
int main() {
    // main() では何もしない
    return 0;
}
