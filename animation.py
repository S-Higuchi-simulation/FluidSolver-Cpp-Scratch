import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from IPython.display import HTML

# 1. データの読み込み
print("Loading CSV data...")
df = pd.read_csv('result.csv')

# データを整形（縦軸: 時間step, 横軸: 場所index）
data_matrix = df.pivot(index='step', columns='index', values='temperature')
steps = data_matrix.index # 存在する時間ステップのリスト (0, 10, 20...)
x = data_matrix.columns   # 場所のリスト (0, 1, ... 99)

# 2. アニメーションの準備
fig, ax = plt.subplots(figsize=(10, 6))

# 初期の空のプロットを作る
line, = ax.plot([], [], lw=2, color='red')

# グラフの装飾
ax.set_xlim(0, 99)
ax.set_ylim(0, 110) # 温度は0〜100度なので少し広めに
ax.set_xlabel('Position (x)')
ax.set_ylabel('Temperature (T)')
ax.set_title('1D Heat Diffusion Simulation')
ax.grid(True)

# 3. 1コマずつ描画する関数
def update(frame):
    # frame番目のステップの温度データを取得
    current_step = steps[frame]
    y = data_matrix.loc[current_step]
    
    # 線を更新
    line.set_data(x, y)
    ax.set_title(f'Time Step: {current_step}')
    return line,

# 4. アニメーション生成（ここがエンジンの心臓部）
print("Generating Animation...")
anim = animation.FuncAnimation(fig, update, frames=len(steps), interval=50, blit=True)

# 5. Colab上で動画として表示
HTML(anim.to_jshtml())