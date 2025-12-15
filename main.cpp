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

    // 3. 初期条件の設定
    // 真ん中の20グリッド分（40〜60番目）を100度にする
    int start_node = N / 2 - 10; // 40
    int end_node = N / 2 + 10;   // 60
    
    for (int i = start_node; i < end_node; ++i) {
        T[i] = 100.0;
    }

    // 4. 時間発展のループ
    // time_step は「何回計算するか」の回数（例えば 1000回）
    int max_steps = 1000;

    // 出力用のヘッダー
    std::cout << "step,index,temperature" << std::endl;

    for (int t = 0; t < max_steps; ++t) {
        
        // --- ここで空間のループを回す ---
        // 端っこ（0とN-1）は固定なので、1 から N-2 まで計算する
        for (int i = 1; i < N - 1; ++i) {
            T_next[i] = T[i] + r * (T[i+1] - 2*T[i] + T[i-1]);
        }
        
        // --- 境界条件の適用 ---
        // 両端の温度を強制的に0度（ディリクレ条件）に固定する
        T_next[0] = 0.0;
        T_next[N - 1] = 0.0;

        // --- 温度の更新 ---
        // 計算した T_next を T にコピーして時間を進める
        T = T_next;

        // --- 結果の出力（100回に1回だけ出す） ---
        if (t % 100 == 0) {
            // とりあえず真ん中の温度だけ出してみる例
            std::cout << t << "," << 50 << "," << T[50] << std::endl;
        }
    }

    std::cout << "Setup complete. Ready to simulate." << std::endl;

    return 0;
}