#include <iostream>
#include <vector>
#include <cmath>
#include <emscripten.h>

// --------------------------------------------------------------------------
// 1. グローバル変数の定義（シミュレーション状態を保持）
// --------------------------------------------------------------------------

// パラメータ
const int N = 100;
const double L = 1.0;
const double dx = L / N;
const double alpha = 0.01;

// シミュレーションの状態を保持するグローバル変数
// （main()関数スコープから外に出す）
std::vector<double> T(N, 0.0);       // 現在の温度
std::vector<double> T_next(N, 0.0); // 次の瞬間の温度

// --------------------------------------------------------------------------
// 2. 初期化関数
// --------------------------------------------------------------------------
void initialize_simulation() {
    // 3. 初期条件の設定
    int start_node = N / 2 - 10;
    int end_node = N / 2 + 10;
    
    for (int i = start_node; i < end_node; ++i) {
        T[i] = 100.0;
    }
    
    T_next = T; // T_nextも初期化
    
    std::cout << "Simulation initialized. Ready for steps." << std::endl;
}


// --------------------------------------------------------------------------
// 3. JavaScriptから呼び出される「1ステップ処理」関数
// --------------------------------------------------------------------------
extern "C" { 
    // ★★★ 時間刻み幅 (dt) を引数として受け取ります ★★★
    EMSCRIPTEN_KEEPALIVE
    void one_step_and_draw(double dt_from_js) { // new_dtをdt_from_jsに名前変更

        // 1. JSから渡された Δt を基に拡散数 r を計算 (dtは引数を使用)
        double r = alpha * dt_from_js / (dx * dx);

        // 安定性チェック (クーラン条件)
        if (r > 0.5) {
            // クーラン条件違反！計算が発散するため、警告を出して計算を中止する
            std::cout << "Unstable (r > 0.5)! Current r: " << r << std::endl;
            return; // 計算中止
        }
        
        // --- 空間のループを回す（1ステップ分） ---
        for (int i = 1; i < N - 1; ++i) {
            T_next[i] = T[i] + r * (T[i+1] - 2*T[i] + T[i-1]);
        }

        // --- 境界条件の適用 ---
        T_next[0] = 0.0;
        T_next[N - 1] = 0.0;

        // --- 温度の更新 ---
        T = T_next;

        // [TODO: 描画処理]

        // 正常な動作確認のため、コンソールに r の値を小さく出力する（デバッグ用）
        std::cout << "Step: r=" << r << ", dt=" << dt_from_js << std::endl; 
    }
}

// --------------------------------------------------------------------------
// 4. main関数（初期化のみ）
// --------------------------------------------------------------------------
int main() {
    initialize_simulation();
    return 0; // main関数はすぐに終了し、ブラウザのフリーズを防ぐ
}