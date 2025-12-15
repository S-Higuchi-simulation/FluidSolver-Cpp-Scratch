#include <iostream>
#include <vector>
#include <cmath>

int main() {
    // 1. パラメータの定義（物理世界のルール設定）
    const int N = 100;       // 空間分割数（100個の点に分ける）
    const double L = 1.0;    // 棒の長さ [m]
    const double dx = L / N; // 空間刻み幅 [m]
    
    // 拡散係数 (alpha) と時間刻み (dt)
    const double alpha = 0.01; // 熱拡散率 [m^2/s]
    const double dt = 0.0001;  // 時間刻み [s] （小さくしないと計算が爆発する）

    // 安定性解析（CFL条件のようなもの）のチェック
    // 拡散数 r = alpha * dt / dx^2 が 0.5 を超えると計算が発散して壊れる
    double r = alpha * dt / (dx * dx);
    
    std::cout << "Diffusion Number r = " << r << std::endl;
    if (r > 0.5) {
        std::cout << "Warning: Unstable! (r > 0.5)" << std::endl;
        return 1;
    }

    // 2. 場の量（ベクトル）の確保
    // T: 現在の温度, T_next: 次の瞬間の温度
    std::vector<double> T(N, 0.0);      // 初期値は全て0度
    std::vector<double> T_next(N, 0.0);

    std::cout << "Setup complete. Ready to simulate." << std::endl;

    return 0;
}