
<!-- saved from url=(0079)http://10.182.198.199/problem2-20181112000000-20181125235959/chutiyan/code.html -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252"><title>Solution.cpp</title>
<script src="./Solution_2_chutiyan_files/prettify.js.download" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="./Solution_2_chutiyan_files/prettify.css">
</head><body onload="prettyPrint()">
<h2>Solution.cpp</h2><div><pre class="prettyprint linenums">#include &lt;cstdio&gt;
#include &lt;cstring&gt;
#define OUTPUT printf // NOSONAR
#ifndef MAX
#define MAX(a, b) ((a) &gt; (b) ? (a) : (b))
#endif

// ** Global
// ------------------------------------------------------------------------------

/**
 * Matrix data
 */
const int MATRIX_MAX_SIZE = 128;
int matrixWidth;
int matrixHeight;
int employeeAmount[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];

/**
 * Max employee amount in each stop during dynamic programing
 */
int busEmployeeAmount[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];

/**
 * Buffer for input
 */
const int BUFFER_SIZE = MATRIX_MAX_SIZE * MATRIX_MAX_SIZE * 2;
char buffer[BUFFER_SIZE];

// ** Read files and initialize data
// ------------------------------------------------------------------------------

void readFile(char* filePath) {
    int offset = 0;
    int position = 0;
    FILE *fp = fopen(filePath, "r");
    fread(buffer, BUFFER_SIZE, 1, fp);
    sscanf(buffer, "%d, %d%n", &amp;matrixWidth, &amp;matrixHeight, &amp;offset);

    position += offset;

    for (int i = 0; i &lt; matrixHeight; ++i) {
        for (int j = 0; j &lt; matrixWidth; ++j) {
            sscanf((buffer + position), "%d,%n", &amp;employeeAmount[i][j], &amp;offset);
            position += offset;
        }
    }
}

// ** DP
// ------------------------------------------------------------------------------

int compute() {
    for (int i = 0; i &lt; matrixHeight; ++i) {
        for (int j = 0; j &lt; matrixWidth; ++j) {
            int currentAmount = 0;
            if (i &gt; 0) {
                currentAmount = busEmployeeAmount[i - 1][j];
            }

            if (j &gt; 0) {
                currentAmount = MAX(busEmployeeAmount[i][j - 1], currentAmount);
            }

            busEmployeeAmount[i][j] = currentAmount + employeeAmount[i][j];
        }
    }

    return busEmployeeAmount[matrixHeight-1][matrixWidth-1];
}

// ** Entry
// ------------------------------------------------------------------------------

int main(int argc, char **argv) {
    readFile(argv[1]);
    int result = compute();
    OUTPUT("%d\n", result);
}
</pre></div>
</body></html>