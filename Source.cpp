#include <conio.h>
#include <locale.h>
#include <math.h>
#include <complex>
#include <iostream>

struct IntPoint2
{
	int _x;
	int _y;

	bool operator== (const IntPoint2& rhs) const noexcept
	{
		if ((_x == rhs._x) && (_y == rhs._y)) return true;
		else return false;
	}

	float Distance(const IntPoint2& rhs) const noexcept
	{
		return static_cast<float>(sqrt(pow((_x - rhs._x), 2) + pow((_y - rhs._y), 2)));
	}
};

std::ostream& operator<< (std::ostream& out, const IntPoint2& point)
{
	out << "(" << point._x << "; " << point._y << ")";
	return out;
}

struct DoublePoint3
{
	double _x;
	double _y;
	double _z;

	bool operator== (const DoublePoint3& rhs) const noexcept
	{
		if ((_x == rhs._x) && (_y == rhs._y) && (_z == rhs._z)) return true;
		else return false;
	}

	double Distance(const DoublePoint3& rhs) const noexcept
	{
		return sqrt(pow((_x - rhs._x), 2) + pow((_y - rhs._y), 2) + pow((_z - rhs._z), 2));
	}
};

std::ostream& operator<< (std::ostream& out, const DoublePoint3& point)
{
	out << "(" << point._x << "; " << point._y << "; " << point._z << ")";
	return out;
}


template <class T>
class Polyline
{
	T* p;
	size_t vertex;

public:
	Polyline();
	Polyline(int count_vertex);
	~Polyline();
	Polyline(const Polyline& polyline);
	size_t Vertex() const noexcept;
	auto Length() const;
	bool operator== (const Polyline& rhs) const noexcept;
	bool operator!= (const Polyline& rhs) const noexcept;
	Polyline operator+ (const Polyline& polyline) noexcept;
	Polyline& operator = (const Polyline& line) noexcept;
	T operator[] (const size_t index) const;
	T& operator[] (const size_t index);
	void AddToEnd(const T& point);
	void AddToBegin(const T& point);
};

template <class T>
std::ostream& operator<< (std::ostream& out, const Polyline<T>& polyline)
{
	out << "Polyline[";
	for (size_t i = 0; i < polyline.Vertex(); ++i)
	{
		if (i != polyline.Vertex() - 1) out << polyline[i] << ", ";
		else out << polyline[i];
	}
	out << "]";
	return out;
}

template <class T>
Polyline<T>::Polyline() : p(nullptr), vertex(0) { }

template <class T>
Polyline<T>::Polyline(int count_vertex)
{
	if (count_vertex < 0) throw "Ошибка подсчёта вершин";
	vertex = (size_t)count_vertex;
	p = new T[count_vertex]();
}

template <class T>
Polyline<T>::~Polyline()
{
	delete[] p;
}

template <class T>
Polyline<T>::Polyline(const Polyline<T>& polyline) : p(new T[polyline.vertex]), vertex(polyline.vertex)
{
	for (size_t i = 0; i < polyline.vertex; ++i)
	{
		p[i] = polyline.p[i];
	}
}

template<class T>
size_t Polyline<T>::Vertex() const noexcept
{
	return vertex;
}

template <class T>
auto Polyline<T>::Length() const
{
	if ((p == nullptr) || (vertex < 2)) throw "Линия пуста";
	auto lenght = p[0].Distance(p[1]);
	for (size_t i = 1; i < vertex - 1; ++i)
	{
		lenght += p[i].Distance(p[i + 1]);
	}
	return lenght;
}

template <>
auto Polyline<std::complex<double>>::Length() const
{
	if ((p == nullptr) || (vertex < 2)) throw "Линия пуста";
	double lenght = 0;
	for (size_t i = 0; i < vertex - 1; ++i)
	{
		lenght += std::abs((p[i + 1] - p[i]));
	}
	return lenght;
}

template <class T>
bool Polyline<T>::operator== (const Polyline<T>& rhs) const noexcept
{
	if (vertex != rhs.vertex) return false;
	for (size_t i = 0; i < vertex; ++i)
	{
		if (!(p[i] == rhs.p[i])) return false;
	}
	return true;
}

template <class T>
bool Polyline<T>::operator!= (const Polyline<T>& rhs) const noexcept
{
	if (vertex != rhs.vertex) return true;
	for (size_t i = 0; i < vertex; ++i)
	{
		if (!(p[i] == rhs.p[i])) return true;
	}
	return false;
}

template <class T>
Polyline<T> Polyline<T>::operator+ (const Polyline<T>& polyline) noexcept
{
	Polyline<T> result(vertex + polyline.vertex);
	for (size_t i = 0; i < vertex; ++i)
	{
		result.p[i] = p[i];
	}
	for (size_t i = 0; i < polyline.vertex; ++i)
	{
		result.p[vertex + i] = polyline.p[i];
	}
	return result;
}

template <class T>
Polyline<T>& Polyline<T>::operator = (const Polyline<T>& line) noexcept
{
	if (this == (&line)) return *this;
	if (p) delete[] p;
	if (line.p)
	{
		p = new T[line.vertex];
		vertex = line.vertex;
		for (size_t i = 0; i < line.vertex; ++i)
		{
			p[i] = line.p[i];
		}
	}
	else
	{
		p = nullptr;
		vertex = 0;
	}
	return *this;
}

template <class T>
T Polyline<T>::operator[] (const size_t index) const
{
	if (index >= vertex) throw "Неправильный индекс";
	return p[index];
}

template <class T>
T& Polyline<T>::operator[] (const size_t index)
{
	if (index >= vertex) throw "Неправильный индекс";
	return p[index];
}

template <class T>
void Polyline<T>::AddToEnd(const T& point)
{
	for (size_t i = 0; i < vertex; ++i)
	{
		if (p[i] == point) throw "Добавление существующей точки";
	}
	vertex += 1;
	T* tmp = new T[vertex];
	for (size_t i = 0; i < vertex - 1; ++i)
	{
		tmp[i] = p[i];
	}
	tmp[vertex - 1] = point;
	if (p != nullptr) delete[] p;
	p = tmp;
}

template <class T>
void Polyline<T>::AddToBegin(const T& point)
{
	for (size_t i = 0; i < vertex; ++i)
	{
		if (p[i] == point) throw "Добавление существующей точки";
	}
	vertex += 1;
	T* tmp = new T[vertex];
	tmp[0] = point;
	for (size_t i = 1; i < vertex; ++i)
	{
		tmp[i] = p[i - 1];
	}
	if (p != nullptr) delete[] p;
	p = tmp;
}

int GetKey()
{
	int key = _getch();
	if ((key == 0) || (key == 224)) key = _getch();
	return key;
}


int Menu1()
{
	std::cout << "\nИзменить первую ломанную - 1\nИзменить вторую ломанную - 2\nПросмотр ломанных - 3\nСравнение ломанных на равенство - 4\nСравнение ломанных на неравенство - 5\nСложение двух ломанных - 6\nНазад - Esc" << std::endl;
	while (true)
	{
		int key = GetKey();
		if ((key == 49) || (key == 50) || (key == 51) || (key == 52) || (key == 53) || (key == 54) || (key == 27)) return key;
	}
}

int Menu2()
{
	std::cout << "\nДобавить точку в начало ломанной - 1\nДобавить точку в конец ломанной - 2\nИзменить вершину по индексу - 3\nРассчитать длину ломанной - 4\nНазад - Esc" << std::endl;
	while (true)

	{
		int key = GetKey();
		if ((key == 49) || (key == 50) || (key == 51) || (key == 52) || (key == 27)) return key;
	}
}

int MainMenu()
{
	std::cout << "Лабораторная работа №2. Ломанные линии" << std::endl;
	std::cout << "Выберете тип точек:" << std::endl;
	std::cout << "\nInt - 1\nDouble - 2\nКомплексные - 3\nВыход - Esc" << std::endl;
	while (true)

	{
		int key = GetKey();
		if ((key == 49) || (key == 50) || (key == 51) || (key == 27)) return key;
	}
}

void PolylineOfPoints2()
{
	Polyline<IntPoint2> line_1, line_2;
	while (true)
	{
		system("cls");
		int m1 = Menu1();
		if (m1 == 49)
		{
			while (true)
			{
				system("cls");
				std::cout << line_1 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					IntPoint2 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y;
					try
					{
						line_1.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					IntPoint2 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y;
					try
					{
						line_1.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << "(" << line_1[index]._x << "," << line_1[index]._y << ")" << std::endl;
						IntPoint2 p;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> p._x >> p._y;
						line_1[index]._x = p._x;
						line_1[index]._y = p._y;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						std::cout << "Длина ломанной: " << line_1.Length() << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(line_1.Length()).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 50)
		{
			while (true)
			{
				system("cls");
				std::cout << line_2 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					IntPoint2 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y;
					try
					{
						line_2.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					IntPoint2 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y;
					try
					{
						line_2.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << "(" << line_2[index]._x << "," << line_2[index]._y << ")" << std::endl;
						IntPoint2 p;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> p._x >> p._y;
						line_2[index]._x = p._x;
						line_2[index]._y = p._y;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						std::cout << "Длина ломанной: " << line_2.Length() << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(line_2.Length()).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 51)
		{
			system("cls");
			std::cout << line_1 << std::endl;
			std::cout << line_2 << std::endl;
			system("pause");
		}

		if (m1 == 52)
		{
			system("cls");
			if (line_1 == line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

			system("pause");
		}

		if (m1 == 53)
		{
			system("cls");
			if (line_1 != line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

		}

		if (m1 == 54)
		{
			system("cls");
			Polyline<IntPoint2> result = line_1 + line_2;
			std::cout << result << std::endl;
			system("pause");
		}

		if (m1 == 27) break;
	}
}

void PolylineOfPoints3()
{
	Polyline<DoublePoint3> line_1, line_2;
	while (true)
	{
		system("cls");
		int m1 = Menu1();
		if (m1 == 49)
		{
			while (true)
			{
				system("cls");
				std::cout << line_1 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					DoublePoint3 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y >> p._z;
					try
					{
						line_1.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					DoublePoint3 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y >> p._z;
					try
					{
						line_1.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << "(" << line_1[index]._x << "," << line_1[index]._y << line_1[index]._z << ")" << std::endl;
						DoublePoint3 p;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> p._x >> p._y >> p._z;
						line_1[index]._x = p._x;
						line_1[index]._y = p._y;
						line_1[index]._z = p._z;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						std::cout << "Длина ломанной: " << line_1.Length() << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(line_1.Length()).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 50)
		{
			while (true)
			{
				system("cls");
				std::cout << line_2 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					DoublePoint3 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y >> p._z;
					try
					{
						line_2.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					DoublePoint3 p;
					std::cout << "Введите координаты точки" << std::endl;
					std::cin >> p._x >> p._y >> p._z;
					try
					{
						line_2.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << "(" << line_2[index]._x << "," << line_2[index]._y << line_2[index]._z << ")" << std::endl;
						DoublePoint3 p;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> p._x >> p._y >> p._z;
						line_2[index]._x = p._x;
						line_2[index]._y = p._y;
						line_2[index]._z = p._z;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						auto length = line_2.Length();
						std::cout << "Длина ломанной: " << length << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(length).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 51)
		{
			system("cls");
			std::cout << line_1 << std::endl;
			std::cout << line_2 << std::endl;
			system("pause");
		}

		if (m1 == 52)
		{
			system("cls");
			if (line_1 == line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

			system("pause");
		}

		if (m1 == 53)
		{
			system("cls");
			if (line_1 != line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

			system("pause");
		}

		if (m1 == 54)
		{
			system("cls");
			Polyline<DoublePoint3> result = line_1 + line_2;
			std::cout << result << std::endl;
			system("pause");
		}

		if (m1 == 27) break;
	}
}

void PolylineOfComplexPoints()
{
	Polyline<std::complex<double>> line_1, line_2;
	while (true)
	{
		system("cls");
		
		int m1 = Menu1();
		if (m1 == 49)
		{
			while (true)
			{
				system("cls");
				std::cout << line_1 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					double im = 0, re = 0;
					std::cout << "Введите координаты точки (действительная, мнимая часть)" << std::endl;
					std::cin >> re >> im;
					std::complex<double> p(re, im);
					try
					{
						line_1.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					double im = 0, re = 0;
					std::cout << "Введите координаты точки (действительная, мнимая часть)" << std::endl;
					std::cin >> re >> im;
					std::complex<double> p(re, im);
					try
					{
						line_1.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << line_1[index] << std::endl;
						double im = 0, re = 0;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> re >> im;
						std::complex<double> p(re, im);
						line_1[index] = p;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						std::cout << "Длина ломанной: " << line_1.Length() << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(line_1.Length()).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 50)
		{
			while (true)
			{
				system("cls");
				std::cout << line_2 << std::endl;
				int m2 = Menu2();
				if (m2 == 49)
				{
					double im = 0, re = 0;
					std::cout << "Введите координаты точки (действительная, мнимая  часть)" << std::endl;
					std::cin >> re >> im;
					std::complex<double> p(re, im);
					try
					{
						line_2.AddToBegin(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 50)
				{
					double im = 0, re = 0;
					std::cout << "Введите координаты точки (действительная, мнимая часть)" << std::endl;
					std::cin >> re >> im;
					std::complex<double> p(re, im);
					try
					{
						line_2.AddToEnd(p);
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 51)
				{
					int index = 0;
					std::cout << "Введите индекс координаты" << std::endl;
					std::cin >> index;
					try
					{
						std::cout << line_2[index] << std::endl;
						double im = 0, re = 0;
						std::cout << "Введите необходимые координаты вершины" << std::endl;
						std::cin >> re >> im;
						std::complex<double> p(re, im);
						line_2[index] = p;
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 52)
				{
					try
					{
						system("cls");
						std::cout << "Длина ломанной: " << line_2.Length() << std::endl;
						std::cout << "Тип возвращаемого значения: " << typeid(line_2.Length()).name() << std::endl;

						system("pause");
					}
					catch (const char* err)
					{
						std::cout << err << std::endl;
						system("pause");
					}
				}
				if (m2 == 27) break;
			}
		}

		if (m1 == 51)
		{
			system("cls");
			std::cout << line_1 << std::endl;
			std::cout << line_2 << std::endl;
			system("pause");
		}

		if (m1 == 52)
		{
			system("cls");
			if (line_1 == line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

			system("pause");
		}

		if (m1 == 53)
		{
			system("cls");
			if (line_1 != line_2) std::cout << "Верно" << std::endl;
			else std::cout << "Неверно" << std::endl;

			system("pause");
		}

		if (m1 == 54)
		{
			system("cls");
			Polyline<std::complex<double>> result = line_1 + line_2;
			std::cout << result << std::endl;
			system("pause");
		}

		if (m1 == 27) break;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	while (true)
	{
		system("cls");
		int polyline_type = MainMenu();

		if (polyline_type == 49) PolylineOfPoints2();

		if (polyline_type == 50) PolylineOfPoints3();

		if (polyline_type == 51) PolylineOfComplexPoints();

		if (polyline_type == 27) break;
	}
	return 0;
}