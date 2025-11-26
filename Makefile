# Compiler
CXX = g++

# -I./inc : Tìm header file của project (main.h, Task*.h, tinyxml2.h)
# -I./lib : Tìm header file của CUDD (cudd.h, cuddObj.hh) -> Đã sửa theo ảnh
# -I/usr/local/include : Đường dẫn dự phòng cho header hệ thống
CXXFLAGS = -std=c++11 -O2 -I./inc -I./lib -I/usr/local/include

# Thư viện liên kết:
# -L/usr/local/lib : Tìm file thư viện biên dịch sẵn (.a/.so) của CUDD
# -lcudd : Link thư viện CUDD
# LƯU Ý: Đã xóa -ltinyxml2 vì ta sẽ biên dịch trực tiếp tinyxml2.cpp
LDFLAGS = -L/usr/local/lib -lcudd

# Danh sách file nguồn (nằm trong thư mục src)
# Đã thêm src/tinyxml2.cpp vào đây vì file này có trong cây thư mục của bạn
SRCS = src/main.cpp \
       src/Task1_PNMLParser.cpp \
       src/Task2_ReachabilityGraph.cpp \
       src/Task3_BDD_SymbolicReach.cpp \
       src/tinyxml2.cpp
#      src/Task4_DeadlockILP.cpp \
#      src/Task5_OptimizeILP.cpp

# Tên file chạy đầu ra
TARGET = pnml_test

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)