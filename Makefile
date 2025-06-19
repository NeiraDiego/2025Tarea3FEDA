CXX = g++
CXXFLAGS = -std=c++17 -O2
SRC = main.cpp HashCerrado.cpp HashAbierto.cpp BSTUsuarios.cpp MurmurHash3.cpp
OUT = experimento

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

