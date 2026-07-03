import os
import sys
import shutil
from PyQt6 import QtCore, QtGui, QtWidgets


def find_mpi_launcher():
    # PATH
    for exe in ("mpirun", "mpiexec"):
        path = shutil.which(exe)
        if path:
            return path

    # Типичные пути Windows
    user_profile = os.environ.get("USERPROFILE", "C:\\Users\\Public")
    program_data = os.environ.get("ProgramData", r"C:\\ProgramData")
    candidates = [
        # MS-MPI
        r"C:\\Program Files\\Microsoft MPI\\Bin\\mpiexec.exe",
        r"C:\\Program Files (x86)\\Microsoft MPI\\Bin\\mpiexec.exe",
        # OpenMPI
        r"C:\\Program Files\\OpenMPI\\bin\\mpirun.exe",
        r"C:\\Program Files (x86)\\OpenMPI\\bin\\mpirun.exe",
        # Scoop (по умолчанию в профиле пользователя)
        os.path.join(user_profile, "scoop", "apps", "openmpi", "current", "bin", "mpirun.exe"),
        os.path.join(user_profile, "scoop", "apps", "msmpi", "current", "bin", "mpiexec.exe"),
        # Chocolatey shim bin
        os.path.join(program_data, "chocolatey", "bin", "mpirun.exe"),
        os.path.join(program_data, "chocolatey", "bin", "mpiexec.exe"),
    ]
    for p in candidates:
        try:
            if p and os.path.exists(p):
                return p
        except Exception:
            pass
    return None


class LogView(QtWidgets.QPlainTextEdit):
    def __init__(self):
        super().__init__()
        self.setReadOnly(True)
        self.setWordWrapMode(QtGui.QTextOption.WrapMode.NoWrap)
        self.setStyleSheet("""
            QPlainTextEdit {
                background-color: #111315;
                color: #c8d1d9;
                font-family: Consolas, Courier New, monospace;
                font-size: 12px;
                border: 1px solid #1f2326;
            }
        """)

    def append_line(self, text: str):
        self.appendPlainText(text.rstrip("\n"))
        self.verticalScrollBar().setValue(self.verticalScrollBar().maximum())


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Лабораторная №8 — MPI")
        self.resize(1200, 720)

        # Тёмная тема минималистичная
        self.setStyleSheet("""
            QWidget { background-color: #0b0d10; color: #c8d1d9; }
            QGroupBox { border: 1px solid #1f2326; margin-top: 10px; }
            QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }
            QLabel { color: #c8d1d9; }
            QLineEdit, QSpinBox { background-color: #111315; color: #c8d1d9; border: 1px solid #1f2326; }
            QCheckBox { background-color: transparent; color: #c8d1d9; }
            QCheckBox::indicator {
                width: 16px; height: 16px; border: 1px solid #3b4046; border-radius: 3px; background: #14171a;
            }
            QCheckBox::indicator:hover { border-color: #4b5056; }
            QCheckBox::indicator:checked { background: #2b6cb0; border-color: #2b6cb0; }
            QPushButton { background-color: #1a1f24; color: #c8d1d9; border: 1px solid #2b3137; padding: 6px 12px; }
            QPushButton:hover { background-color: #242a31; }
            QPushButton:disabled { color: #7a8087; border-color: #1f2326; }
            QStatusBar { background-color: #0b0d10; color: #7a8087; }
        """)

        central = QtWidgets.QWidget()
        layout = QtWidgets.QVBoxLayout(central)
        self.setCentralWidget(central)

        # Верхняя панель настроек
        cfg_box = QtWidgets.QGroupBox("Параметры запуска")
        cfg_layout = QtWidgets.QGridLayout(cfg_box)

        self.spin_procs = QtWidgets.QSpinBox()
        self.spin_procs.setRange(2, 1024)
        self.spin_procs.setValue(32)
        self.spin_procs.setSingleStep(1)
        self.spin_groups = QtWidgets.QSpinBox()
        self.spin_groups.setRange(1, 256)
        self.spin_groups.setValue(4)
        self.spin_groups.setSingleStep(1)
        self.chk_oversub = QtWidgets.QCheckBox("--oversubscribe")

        self.spin_matrix = QtWidgets.QSpinBox()
        self.spin_matrix.setRange(100, 5000)
        self.spin_matrix.setValue(500)
        self.spin_matrix.setSingleStep(50)

        cfg_layout.addWidget(QtWidgets.QLabel("Процессов:"), 0, 0)
        cfg_layout.addWidget(self.spin_procs, 0, 1)
        cfg_layout.addWidget(QtWidgets.QLabel("Групп:"), 1, 0)
        cfg_layout.addWidget(self.spin_groups, 1, 1)
        cfg_layout.addWidget(QtWidgets.QLabel("Размер матрицы:"), 2, 0)
        cfg_layout.addWidget(self.spin_matrix, 2, 1)
        cfg_layout.addWidget(self.chk_oversub, 3, 0, 1, 2)


        # Пресеты как в lab8_runner
        presets_box = QtWidgets.QGroupBox("Пресеты")
        presets_layout = QtWidgets.QHBoxLayout(presets_box)
        self.btn_quick = QtWidgets.QPushButton("normal: 8×2")
        self.btn_normal = QtWidgets.QPushButton("quick: 4×1")
        self.btn_max = QtWidgets.QPushButton("max: 64×8 + oversub")
        presets_layout.addWidget(self.btn_quick)
        presets_layout.addWidget(self.btn_normal)
        presets_layout.addWidget(self.btn_max)

        self.btn_run = QtWidgets.QPushButton("Запустить")
        self.btn_clear = QtWidgets.QPushButton("Очистить лог")

        btns = QtWidgets.QHBoxLayout()
        btns.addStretch()
        btns.addWidget(self.btn_run)
        btns.addWidget(self.btn_clear)

        # Лог
        self.log = LogView()

        layout.addWidget(cfg_box)
        layout.addWidget(presets_box)
        layout.addLayout(btns)
        layout.addWidget(self.log, 1)

        # Статус-бар
        self.status = self.statusBar()

        # Сигналы
        self.btn_quick.clicked.connect(lambda: self.apply_preset(8, 2, False))
        self.btn_normal.clicked.connect(lambda: self.apply_preset(4, 1, False))
        self.btn_max.clicked.connect(lambda: self.apply_preset(64, 8, True))
        self.btn_run.clicked.connect(self.run_clicked)
        self.btn_clear.clicked.connect(lambda: self.log.setPlainText("") )

        # Процесс
        self.proc = QtCore.QProcess(self)
        self.proc.setProcessChannelMode(QtCore.QProcess.ProcessChannelMode.MergedChannels)
        # Принудительно включаем UTF-8 для дочернего процесса Python
        env = QtCore.QProcessEnvironment.systemEnvironment()
        env.insert("PYTHONIOENCODING", "utf-8")
        env.insert("PYTHONUTF8", "1")
        self.proc.setProcessEnvironment(env)
        # Обеспечиваем стабильный рабочий каталог (папка скрипта)
        self.proc.setWorkingDirectory(os.path.dirname(__file__))
        self.proc.readyReadStandardOutput.connect(self.on_read_output)
        self.proc.finished.connect(self.on_finished)

        self.mpi_launcher = find_mpi_launcher()
        if not self.mpi_launcher:
            self.status.showMessage("MPI launcher не найден (mpirun/mpiexec). Установите MS-MPI или OpenMPI.")
            self.log.append_line("Ошибка: mpirun/mpiexec не найден. Установите MS-MPI (Windows) или OpenMPI.")

    def apply_preset(self, procs: int, groups: int, oversub: bool):
        self.spin_procs.setValue(procs)
        self.spin_groups.setValue(groups)
        self.chk_oversub.setChecked(oversub)
        # матрица по умолчанию
        self.spin_matrix.setValue(500)

    def run_clicked(self):
        if self.proc.state() != QtCore.QProcess.ProcessState.NotRunning:
            self.status.showMessage("Уже выполняется…")
            return

        procs = int(self.spin_procs.value())
        groups = int(self.spin_groups.value())
        oversub = self.chk_oversub.isChecked()
        matrix_size = int(self.spin_matrix.value())

        if procs < 2 or groups < 1:
            self.status.showMessage("Минимум: 2 процесса, 1 группа")
            return

        launcher = self.mpi_launcher or find_mpi_launcher()
        if not launcher:
            self.status.showMessage("mpirun/mpiexec не найден")
            self.log.append_line("mpirun/mpiexec не найден. Установите MS-MPI или OpenMPI и перезапустите.")
            return

        self.log.append_line("Запуск…")
        self.status.showMessage("Выполняется…")

        args = []
        launcher_name = os.path.basename(launcher).lower()
        # oversubscribe только для OpenMPI/mpirun
        if oversub and "mpirun" in launcher_name:
            args.append("--oversubscribe")
        # -n для mpiexec (MS-MPI), -np для mpirun
        np_flag = "-n" if "mpiexec" in launcher_name else "-np"
        # Запускаем Python в UTF-8 режиме для корректного вывода кириллицы
        args.extend([np_flag, str(procs), sys.executable, "-X", "utf8", os.path.join(os.path.dirname(__file__), "complete_mpi_lab.py"), str(groups), str(matrix_size)])


        self.proc.start(launcher, args)

    def on_read_output(self):
        raw = self.proc.readAllStandardOutput().data()
        text = None
        # Пытаемся декодировать как UTF-8, иначе cp1251 (Windows), иначе latin-1
        for enc in ("utf-8", "cp1251", "latin-1"):
            try:
                text = raw.decode(enc)
                break
            except UnicodeDecodeError:
                continue
        if text is None:
            # В крайнем случае не теряем символы
            text = raw.decode("utf-8", errors="replace")
        for line in text.splitlines():
            self.log.append_line(line)

    def on_finished(self, code, status):
        self.status.showMessage(f"Завершено с кодом {code}")



def main():
    app = QtWidgets.QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()

