Thêm script Python để sinh ra 2 file test lớn

1.  **`complex_parallel.pnml`**: Mạng gồm 12 tiến trình chạy song song.
      * **Độ phức tạp:** Số trạng thái = $2^{12} = 4096$ markings. (Gấp nhiều lần so với 2-4 markings ở ví dụ cũ).
      * Thử thách BFS (về thời gian/bộ nhớ) và kiểm chứng khả năng nén của BDD.
2.  **`complex_deadlock.pnml`**: Bài toán "Triết gia ăn tối" (Dining Philosophers) với 5 triết gia.
      * Có deadlock tiềm ẩn (khi tất cả cùng cầm nĩa bên trái).
      * Kiểm tra Task 4 cực tốt.


### Chạy thử nghiệm và Đánh giá

1.  **Biên dịch lại code (để nhận tham số từ `main`)**:

    ```bash
    g++ -std=c++17 -Iinc -Icudd/local_install/include src/*.cpp -o build/pnml_test_v2 -Lcudd/local_install/lib -lcudd
    ```

2.  **Sinh file test**:

    ```bash
    python3 gen_test.py
    ```

    *(tạo ra `test/complex_parallel.pnml` và `test/complex_deadlock.pnml`)*

3.  **Chạy Test 1: Parallel (Kiểm tra hiệu năng & Số lớn)**

    ```bash
    ./build/pnml_test_v2 test/complex_parallel.pnml
    ```

    **Kỳ vọng:**

      * **Reachable markings:** `4096`. (Nếu BFS in ra hết màn hình thì hơi dài, nhưng số lượng phải đúng).
      * **BDD Time:** Rất nhanh (có thể \< 0.01s) vì BDD nén các trạng thái song song cực tốt.
      * **BFS Time:** Sẽ chậm hơn đáng kể so với BDD.
      * **Task 5:** Max Value sẽ rất lớn 
      * **Deadlock:** NO DEADLOCK FOUND.

4.  **Chạy Test 2: Dining Philosophers (Kiểm tra Deadlock)**

    ```bash
    ./build/pnml_test_v2 test/complex_deadlock.pnml
    ```

    **Kỳ vọng:**

      * **Task 4:** `[RESULT] DEADLOCK FOUND!` (Khi tất cả các triết gia đều cầm nĩa trái `HasL`, không ai lấy được nĩa phải).
      * **Reachable markings:** Số lượng markings sẽ ít hơn trường hợp parallel nhưng cấu trúc phức tạp hơn.

