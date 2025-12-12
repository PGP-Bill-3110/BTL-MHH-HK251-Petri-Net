
Về cấu trúc:
Folder source code nhóm em bao gồm:
|_readme
|_Task_1_PNMLParser
|_Task_2_ReachabilityGraph
|_Task_3_BDD_Symbolic
|_Task_4_DeadlockILP
|_Task_5_OptimizeILP

Với mỗi task là 1 folder để có thể chạy và kiểm tra kết quả từng task

////////////////////////
//Task 1 - 2
1. Giới thiệu
-Task 1: Đọc và parse file PNML để lấy các thành phần của Petri Net (places, transitions, arcs) theo từng net riêng biệt.
-Task 2: Từ các net đã thu được trước đó, xây dựng bằng thuật toán BFS (Breadth First Search) để chương trình tính toán ra các reachable markings.
...

2. Cách chạy chương trình
...

3. Chi tiết và yêu cầu cho chương trình
Trong file main.cpp, đường dẫn file PNML được đặt ở dòng:
    string filename = "test/phylosopher.pnml";
Có thể chỉnh lại đường dẫn theo file tùy ý, các file được dùng cho testcase sẽ được để trong folder test

Cấu trúc của file PNML sẽ có khuôn tối thể như sau:
<pnml>
  <net id="n1">
    <page id="p1">

      <place id="P1">
        <name><text>P1</text></name>
        <initialMarking><text>1</text></initialMarking>
      </place>
      ... //more places

      <transition id="T1">
        <name><text>T1</text></name>
      </transition>
      ... //more transitions

      <arc id="A1" source="P1" target="T1">
        <inscription><text>1</text></inscription>
      </arc>
      ... //more arcs

    </page>
  </net>

  <net id="n2">
    <page id="p2">

      <place id="P1">
        <name><text>P1</text></name>
        <initialMarking><text>1</text></initialMarking>
      </place>
      ... //more places

      <transition id="T1">
        <name><text>T1</text></name>
      </transition>
      ... //more transitions

      <arc id="A1" source="P1" target="T1">
        <inscription><text>1</text></inscription>
      </arc>
      ... //more arcs

    </page>
  </net>

  ... //more nets
</pnml>

Yêu cầu: chương trình chương trình sử dụng C++17, thư viện tinyxml2, ...

4. Output của chương trình
...

/////////////////////////////////////////////////////////////

Task 3:

Task 3 phải được chạy trên hệ điều hành Linux do sử dụng thư viện CUDD.

Lệnh để biên dịch: g++ -std=c++11 -O2 -I./inc -I./lib -I/usr/local/include -o pnml_test src/main.cpp src/Task1_PNMLParser.cpp src/Task2_ReachabilityGraph.cpp src/Task3_BDD_SymbolicReach.cpp src/tinyxml2.cpp -L/usr/local/lib -lcudd

Lệnh để chạy:  ./pnml_test
