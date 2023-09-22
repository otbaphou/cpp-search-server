// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
#include <iostream>
#include <string>

using namespace std;
int main()
{
	int answer = 0;
	for (int i = 1; i < 1000; i++)
	{
		string s = to_string(i);

		for (int e = 0; e < s.size(); e++)
		{
			if (s[e] == '3')
			{
				answer++;
				continue;
			}
		}
	}
    cout << answer;
}
// Закомитьте изменения и отправьте их в свой репозиторий.
