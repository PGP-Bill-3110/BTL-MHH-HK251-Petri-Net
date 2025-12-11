Làm ở các branch, ổn r thì mới pull vào main

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

