
# Symbolic and Algebraic Reasoning in Petri Nets

Chương trình thực hiện phân tích mạng Petri (Petri Nets) sử dụng cả phương pháp duyệt không gian trạng thái tường minh (BFS) và phương pháp biểu tượng (Symbolic) sử dụng BDD (Binary Decision Diagrams).

## Tính năng chính
* **Task 1:** Đọc và phân tích file Petri Net chuẩn PNML.
* **Task 2:** Tính toán tập trạng thái đạt tới (Reachability Set) bằng thuật toán BFS (Explicit).
* **Task 3:** Tính toán tập trạng thái đạt tới bằng BDD (Symbolic) để xử lý bùng nổ không gian trạng thái.
* **Task 4:** Phát hiện Deadlock sử dụng kết hợp BDD và các ràng buộc logic.
* **Task 5:** Tối ưu hóa hàm mục tiêu tuyến tính trên tập trạng thái đạt tới.

## Yêu cầu hệ thống
* **C++ Compiler:** Hỗ trợ chuẩn C++17 (g++).
* **Python 3:** Để chạy script sinh dữ liệu test.
* **Thư viện CUDD:** Cần được cài đặt và liên kết đúng đường dẫn (trong thư mục `cudd/`).

## Cấu trúc thư mục
* `src/`: Mã nguồn C++ (`.cpp`).
* `inc/`: Các file header (`.h`).
* `test/`: Chứa các file PNML dùng để kiểm thử.
* `cudd/`: Thư viện CUDD (Colorado University Decision Diagram).
* `gen_test.py`: Script Python để sinh các test case phức tạp.

---

## Hướng dẫn Biên dịch (Compilation)

Tại thư mục gốc của dự án, sử dụng lệnh sau để biên dịch chương trình (liên kết với thư viện CUDD):

```bash
g++ -std=c++17 -Iinc -Icudd/local_install/include src/*.cpp -o build/pnml_test_v2 -Lcudd/local_install/lib -lcudd
````

> **Lưu ý:** Đảm bảo thư mục `build/` đã tồn tại trước khi chạy lệnh biên dịch (`mkdir -p build`).

-----

## Hướng dẫn Chạy Thử nghiệm (Testing)

### 1\. Sinh dữ liệu Test phức tạp (Stress Testing)

Để kiểm chứng hiệu năng của BDD so với BFS và khả năng phát hiện Deadlock, chúng tôi sử dụng script Python để sinh ra các mạng Petri lớn.

Chạy lệnh sau để tạo file test:

```bash
python3 gen_test.py
```

Lệnh này sẽ tạo ra 2 file trong thư mục `test/`:

1.  **`complex_parallel.pnml`**: Mô phỏng 12 tiến trình chạy song song.
      * *Không gian trạng thái:* $2^{12} = 4096$ markings.
      * *Mục đích:* Kiểm tra hiệu năng nén trạng thái của BDD.
2.  **`complex_deadlock.pnml`**: Bài toán "Triết gia ăn tối" (Dining Philosophers) với 5 triết gia.
      * *Đặc điểm:* Có deadlock tiềm ẩn (khi tất cả cùng cầm nĩa trái).
      * *Mục đích:* Kiểm tra tính năng phát hiện Deadlock (Task 4).

### 2\. Chạy Test Case 1: Hệ thống song song (Parallel Processes)

Kịch bản này kiểm tra khả năng xử lý bùng nổ không gian trạng thái.

**Lệnh chạy:**

```bash
./build/pnml_test_v2 test/complex_parallel.pnml
```

**Kết quả kỳ vọng:**

  * **Reachable markings (Task 2 & 3):** `4096`. (Hai phương pháp phải ra cùng kết quả).
  * **Hiệu năng:** Thời gian chạy BDD (Task 3) nhanh hơn đáng kể so với BFS (Task 2) (thường \< 0.01s).
  * **Task 4 (Deadlock):** `NO DEADLOCK FOUND`.
  * **Task 5 (Optimization):** Tìm được giá trị tối ưu rất lớn (do trọng số random lớn).

### 3\. Chạy Test Case 2: Triết gia ăn tối (Dining Philosophers)

Kịch bản này kiểm tra khả năng phát hiện lỗi hệ thống (Deadlock).

**Lệnh chạy:**

```bash
./build/pnml_test_v2 test/complex_deadlock.pnml
```

**Kết quả kỳ vọng:**

  * **Task 4 (Deadlock):** Báo `[RESULT] DEADLOCK FOUND!`.
      * Hệ thống sẽ chỉ ra trạng thái deadlock (ví dụ: các vị trí `HasL` đều có token).
  * **Reachable markings:** Số lượng trạng thái ít hơn trường hợp parallel (82 markings) nhưng cấu trúc graph phức tạp hơn.

### 4\. Chạy với file mẫu cơ bản

Bạn cũng có thể chạy với các file ví dụ nhỏ có sẵn:

```bash
./build/pnml_test_v2 test/example_2.pnml
```

-----

## Giải quyết sự cố (Troubleshooting)

  * **Lỗi `error while loading shared libraries: libcudd.so`**:
      * Cần thêm đường dẫn thư viện vào biến môi trường:
        ```bash
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/cudd/local_install/lib
        ```
  * **Lỗi `Segmentation fault`**:
      * Đảm bảo file PNML đúng định dạng hoặc được sinh ra từ `gen_test.py`.
      * Đảm bảo đã biên dịch lại code mới nhất sau khi sửa lỗi Parser.

<!-- end list -->

```
```
