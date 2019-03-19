# ifndef PROGRAM_INCLUDE_WINDOW_H_
# define PROGRAM_INCLUDE_WINDOW_H_

# include <vector>

using namespace std;

enum WindowType { None, Hann, Hamming };

vector<double> getWindow(WindowType windowType, int windowSize);

#endif // PROGRAM_INCLUDE_WINDOW_H_
