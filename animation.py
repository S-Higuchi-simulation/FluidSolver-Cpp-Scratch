import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# 1. データの読み込み
print("Loading CSV data...")
df = pd.read_csv('result.csv')

# データを整形
data_matrix = df.pivot(index='step', columns='index', values='temperature')
steps = data_matrix.index
x = data_matrix.columns

# 2. アニメーションの準備
fig, ax = plt.subplots(figsize=(10, 6))
line, = ax.plot([], [], lw=2, color='red')

ax.set_xlim(0, 99)
ax.set_ylim(0, 110)
ax.set_xlabel('Position (x)')
ax.set_ylabel('Temperature (T)')
ax.set_title('1D Heat Diffusion Simulation')
ax.grid(True)

# 3. 更新関数
def update(frame):
    current_step = steps[frame]
    y = data_matrix.loc[current_step]
    line.set_data(x, y)
    ax.set_title(f'Time Step: {current_step}')
    return line,

# 4. アニメーション生成
print("Generating Animation...")
anim = animation.FuncAnimation(fig, update, frames=len(steps), interval=50, blit=True)

# 5. GIFとして保存 (Pillowを使用)
print("Saving as diffusion.gif ...")
anim.save('diffusion.gif', writer='pillow', fps=15)
print("Done!")