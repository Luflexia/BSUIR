import tkinter as tk
from tkinter import ttk
import random
import math

MAZE_WIDTH_CM = 26
MAZE_HEIGHT_CM = 19
DEFAULT_WALL_SCALE = 1.0
DEFAULT_GRID_COLS = 26
DEFAULT_GRID_ROWS = 19
CELL_SIZE_PX = 30
HOLE_RADIUS_PX = 8

class MazeGenerator:
    def __init__(self, cols, rows):
        self.cols = cols
        self.rows = rows
        self.maze = [[{'N': True, 'S': True, 'E': True, 'W': True} for _ in range(cols)] for _ in range(rows)]
        self.visited = [[False for _ in range(cols)] for _ in range(rows)]

    def generate(self, randomize_start=False):
        if randomize_start:
            start_x = random.randint(0, self.cols-1)
            start_y = random.randint(0, self.rows-1)
        else:
            start_x = 0
            start_y = 0
        self._carve_passages_from(start_x, start_y)
        self._remove_dead_ends()
        return self.maze

    def _carve_passages_from(self, cx, cy):
        self.visited[cy][cx] = True
        directions = [('N', (0, -1)), ('S', (0, 1)), ('E', (1, 0)), ('W', (-1, 0))]
        random.shuffle(directions)
        for direction, (dx, dy) in directions:
            nx, ny = cx + dx, cy + dy
            if 0 <= nx < self.cols and 0 <= ny < self.rows and not self.visited[ny][nx]:
                self.maze[cy][cx][direction] = False
                opposite = {'N': 'S', 'S': 'N', 'E': 'W', 'W': 'E'}[direction]
                self.maze[ny][nx][opposite] = False
                self._carve_passages_from(nx, ny)

    def _remove_dead_ends(self):
        for y in range(self.rows):
            for x in range(self.cols):
                while self._is_dead_end(x, y):
                    for direction, (dx, dy) in [('N', (0, -1)), ('S', (0, 1)), ('E', (1, 0)), ('W', (-1, 0))]:
                        nx, ny = x + dx, y + dy
                        if 0 <= nx < self.cols and 0 <= ny < self.rows:
                            if self.maze[y][x][direction]:
                                self.maze[y][x][direction] = False
                                opposite = {'N': 'S', 'S': 'N', 'E': 'W', 'W': 'E'}[direction]
                                self.maze[ny][nx][opposite] = False
                                break

    def _is_dead_end(self, x, y):
        walls = self.maze[y][x]
        return sum(walls.values()) == 3

class MazeApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Генератор лабиринта 19x26 см")
        
        # Основные параметры
        self.wall_scale = DEFAULT_WALL_SCALE
        self.grid_cols = 20  # Начальное значение
        self.grid_rows = 15  # Начальное значение
        self.cell_size = 30  # Начальное значение
        self.holes_density = 0.5  # Начальное значение плотности дырок
        
        # Создаем фреймы для группировки элементов
        self.controls_frame = tk.Frame(self)
        self.controls_frame.pack(fill='x', padx=10, pady=5)
        
        # Ползунок сложности
        self.complexity_frame = tk.LabelFrame(self.controls_frame, text="Сложность лабиринта")
        self.complexity_frame.pack(fill='x', pady=5)
        self.complexity_scale = ttk.Scale(
            self.complexity_frame,
            from_=0, to=1,
            orient='horizontal',
            value=0.5,
            command=self.on_complexity_change
        )
        self.complexity_scale.pack(fill='x', padx=10, pady=5)
        
        # Ползунок количества дырок
        self.holes_frame = tk.LabelFrame(self.controls_frame, text="Количество дырок")
        self.holes_frame.pack(fill='x', pady=5)
        self.holes_scale = ttk.Scale(
            self.holes_frame,
            from_=0, to=1,
            orient='horizontal',
            value=self.holes_density,
            command=self.on_holes_change
        )
        self.holes_scale.pack(fill='x', padx=10, pady=5)
        
        # Кнопки управления
        self.btn_frame = tk.Frame(self.controls_frame)
        self.btn_frame.pack(fill='x', pady=5)
        self.gen_maze_btn = tk.Button(self.btn_frame, text="Новый лабиринт", command=self.generate_maze)
        self.gen_maze_btn.pack(side='left', padx=5)
        self.gen_holes_btn = tk.Button(self.btn_frame, text="Добавить дырки", command=self.generate_holes)
        self.gen_holes_btn.pack(side='left', padx=5)
        
        # Холст для рисования лабиринта
        self.canvas = tk.Canvas(
            self,
            width=self.grid_cols * self.cell_size,
            height=self.grid_rows * self.cell_size,
            bg='white'
        )
        self.canvas.pack(padx=10, pady=10)
        
        # Инициализация лабиринта
        self.maze = None
        self.holes = []
        
        # Теперь можно безопасно вызвать on_complexity_change
        self.on_complexity_change(0.5)
        self.generate_maze()

    def on_complexity_change(self, val):
        complexity = float(val)
        min_cells = 6
        max_cells = 20
        self.grid_cols = int(min_cells + (max_cells - min_cells) * complexity)
        self.grid_rows = int(self.grid_cols * (MAZE_HEIGHT_CM / MAZE_WIDTH_CM))
        # Calculate passage and wall width so that the sum of all walls fits the maze size
        # Total wall count horizontally: grid_cols+1, vertically: grid_rows+1
        # Let passage_width_cm + wall_width_cm = cell_width_cm
        cell_width_cm = MAZE_WIDTH_CM / self.grid_cols
        cell_height_cm = MAZE_HEIGHT_CM / self.grid_rows
        # Let's use the smaller cell size for square cells
        cell_cm = min(cell_width_cm, cell_height_cm)
        # Let wall_width_cm be a fraction of cell_cm, but adjust so that (grid_cols+1)*wall + grid_cols*passage = MAZE_WIDTH_CM
        # Let wall_width_cm = x, passage_width_cm = cell_cm - x
        # (grid_cols+1)*x + grid_cols*(cell_cm-x) = MAZE_WIDTH_CM
        # (grid_cols+1)*x + grid_cols*cell_cm - grid_cols*x = MAZE_WIDTH_CM
        # x + grid_cols*cell_cm = MAZE_WIDTH_CM
        # x = MAZE_WIDTH_CM - grid_cols*cell_cm
        # But cell_cm = MAZE_WIDTH_CM / grid_cols, so grid_cols*cell_cm = MAZE_WIDTH_CM, so x = 0
        # Instead, let's set wall_width_cm as a small fraction, e.g. 0.7 cm, and passage as the rest
        wall_width_cm = max(0.5, min(1.0, cell_cm * 0.18))
        passage_width_cm = cell_cm - wall_width_cm
        self.wall_width_cm = wall_width_cm
        self.passage_width_cm = passage_width_cm
        # Update cell size for display
        cell_size_by_width = (MAZE_WIDTH_CM * 30) / self.grid_cols
        cell_size_by_height = (MAZE_HEIGHT_CM * 30) / self.grid_rows
        self.cell_size = int(min(cell_size_by_width, cell_size_by_height))
        self.wall_scale = self.wall_width_cm
        dimensions_text = f"Сложность лабиринта\n"
        dimensions_text += f"Толщина стен: {self.wall_width_cm:.2f} см\n"
        dimensions_text += f"Ширина проходов: {self.passage_width_cm:.2f} см"
        self.complexity_frame.config(text=dimensions_text)
        self.canvas.config(
            width=self.grid_cols * self.cell_size,
            height=self.grid_rows * self.cell_size
        )
        if self.maze:
            self.generate_maze()

    def generate_maze(self):
        gen = MazeGenerator(self.grid_cols, self.grid_rows)
        self.maze = gen.generate(randomize_start=True)
        self.maze[0][0]['N'] = True
        self.maze[0][0]['W'] = True
        self.maze[self.grid_rows-1][self.grid_cols-1]['S'] = True
        self.maze[self.grid_rows-1][self.grid_cols-1]['E'] = True
        self.holes = []
        self.redraw()

    def on_holes_change(self, val):
        self.holes_density = float(val)
        self.holes_frame.config(text=f"Количество дырок ({int(self.holes_density * 100)}%)")
        if self.maze:
            self.generate_holes()

    def generate_holes(self):
        if not self.maze:
            return
            
        forbidden = set()
        # Исключаем только стартовую и финишную точки
        forbidden.add((0, 0))  # Старт
        forbidden.add((self.grid_cols-1, self.grid_rows-1))  # Финиш
        
        valid_cells = []
        for y in range(self.grid_rows):
            for x in range(self.grid_cols):
                if (x, y) in forbidden: continue
                cell = self.maze[y][x]
                # Проверяем, есть ли проход в этой клетке
                if (not cell['N'] and not cell['S']) or (not cell['E'] and not cell['W']):
                    valid_cells.append((x, y))
        
        # Количество дырок зависит от плотности
        max_holes = len(valid_cells)
        hole_count = int(max_holes * self.holes_density)
        self.holes = random.sample(valid_cells, hole_count) if valid_cells else []
        self.redraw()

    def redraw(self):
        if not self.maze:
            return
            
        self.canvas.delete('all')
        cell_px = self.cell_size
        wall = max(2, int(6 * self.wall_scale))
        
        # Рисуем стены с разделителями
        for y in range(self.grid_rows):
            for x in range(self.grid_cols):
                cx, cy = x * cell_px, y * cell_px
                cell = self.maze[y][x]
                
                # Проверяем соседние стены для добавления разделителей
                if cell['N']:
                    self.canvas.create_line(cx, cy, cx+cell_px, cy, width=wall, fill='black')
                    # Добавляем разделители только если есть соседние стены
                    if y > 0 and self.maze[y-1][x]['S']:
                        for i in range(1, 3):
                            mark_x = cx + (cell_px * i) // 3
                            self.canvas.create_line(mark_x, cy-2, mark_x, cy+2, width=1, fill='red')
                
                if cell['W']:
                    self.canvas.create_line(cx, cy, cx, cy+cell_px, width=wall, fill='black')
                    # Добавляем разделители только если есть соседние стены
                    if x > 0 and self.maze[y][x-1]['E']:
                        for i in range(1, 3):
                            mark_y = cy + (cell_px * i) // 3
                            self.canvas.create_line(cx-2, mark_y, cx+2, mark_y, width=1, fill='red')
                
                if y == self.grid_rows-1 and cell['S']:
                    self.canvas.create_line(cx, cy+cell_px, cx+cell_px, cy+cell_px, width=wall, fill='black')
                    # Добавляем разделители на нижней границе если есть соседние стены
                    if y < self.grid_rows-1 and self.maze[y+1][x]['N']:
                        for i in range(1, 3):
                            mark_x = cx + (cell_px * i) // 3
                            self.canvas.create_line(mark_x, cy+cell_px-2, mark_x, cy+cell_px+2, width=1, fill='red')
                
                if x == self.grid_cols-1 and cell['E']:
                    self.canvas.create_line(cx+cell_px, cy, cx+cell_px, cy+cell_px, width=wall, fill='black')
                    # Добавляем разделители на правой границе если есть соседние стены
                    if x < self.grid_cols-1 and self.maze[y][x+1]['W']:
                        for i in range(1, 3):
                            mark_y = cy + (cell_px * i) // 3
                            self.canvas.create_line(cx+cell_px-2, mark_y, cx+cell_px+2, mark_y, width=1, fill='red')
        
        # Рисуем дырки со случайным смещением
        for (x, y) in self.holes:
            cell = self.maze[y][x]
            if not cell['N'] and not cell['S']:  # Вертикальный проход
                cx = x * cell_px + cell_px//2
                cy = y * cell_px + cell_px//2 + random.randint(-cell_px//4, cell_px//4)
            else:  # Горизонтальный проход
                cx = x * cell_px + cell_px//2 + random.randint(-cell_px//4, cell_px//4)
                cy = y * cell_px + cell_px//2
            
            hole_radius = min(cell_px // 3, int(cell_px * 0.4))
            self.canvas.create_oval(cx-hole_radius, cy-hole_radius,
                                  cx+hole_radius, cy+hole_radius,
                                  fill='gray', outline='black')
        
        # Рисуем старт и финиш
        start_x, start_y = cell_px//2, cell_px//2
        finish_x = (self.grid_cols-0.5) * cell_px
        finish_y = (self.grid_rows-0.5) * cell_px
        
        self.canvas.create_text(start_x, start_y, text="S", fill="green", font=("Arial", 14, "bold"))
        self.canvas.create_text(finish_x, finish_y, text="F", fill="red", font=("Arial", 14, "bold"))
        
        # Рисуем рамку
        self.canvas.create_rectangle(
            0, 0,
            self.grid_cols*cell_px,
            self.grid_rows*cell_px,
            width=2,
            outline='blue'
        )

if __name__ == "__main__":
    app = MazeApp()
    app.mainloop()