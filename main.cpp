#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
 * Computes the Longest Common Subsequence (LCS) using a banded dynamic programming approach.
 * This is optimized for cases where the two strings are "close" to each other within a bounded edit distance.
 * close because are result from a length-n sequence with deletions.
 *
 * Parameters:
 *   - s1: First input string.
 *   - s2: Second input string.
 *   - t: The threshold defining the number of deletions
 *   - dp: A reference to the DP table to store intermediate LCS values.
 *
 * Returns:
 *   - The length of the LCS.
 */
int findLCS(const string &s1, const string &s2, int t, vector<vector<int>> &dp) {
    int m1 = s1.size(), m2 = s2.size();
    dp.assign(m1 + 1, vector<int>(m2 + 1, 0));
    int band = 2 * t + 2; // extra margin // like "hamming distance"
    for (int i = 1; i <= m1; i++) {
        int jStart = max(1, i - band);
        int jEnd = min(m2, i + band);
        for (int j = jStart; j <= jEnd; j++) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    return dp[m1][m2];
}

/*
 * Reconstructs one possible LCS sequence from the computed DP table.
 *
 * Parameters:
 *   - s1: First input string.
 *   - s2: Second input string.
 *   - dp_vec: The computed DP table containing LCS lengths.
 *
 * Returns:
 *   - A string representing one LCS of s1 and s2.
 */
string backtrackLCS(const string &s1, const string &s2, const vector<vector<int>> &dp_vec) {
    int i = s1.size(), j = s2.size();
    string lcs;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            lcs.push_back(s1[i - 1]);
            i--; j--;
        } else {
            if (dp_vec[i - 1][j] > dp_vec[i][j - 1])
                i--;
            else
                j--;
        }
    }
    reverse(lcs.begin(), lcs.end());
    return lcs;
}

/*
 * Constructs the Shortest Common Super-sequence (SCS) from s1, s2, and their LCS.
 *
 * Parameters:
 *   - s1: First input string.
 *   - s2: Second input string.
 *   - lcs: The longest common subsequence of s1 and s2.
 *
 * Returns:
 *   - The shortest common super-sequence of s1 and s2.
 */
string buildSCS(const string &s1, const string &s2, const string &lcs) {
    int i = 0, j = 0, p = 0;
    string scs;
    while (i < s1.size() && j < s2.size() && p < lcs.size()) {
        while (i < s1.size() && s1[i] != lcs[p]) {//enter letters that are only in s1
            scs.push_back(s1[i++]);
        }
        while (j < s2.size() && s2[j] != lcs[p]) {//enter letters that are only in s2
            scs.push_back(s2[j++]);
        }
        scs.push_back(lcs[p]);
        i++; j++; p++;
    }
    while (i < s1.size()) { //enter the rest from s1
        scs.push_back(s1[i++]);
    }
    while (j < s2.size()) {//enter the rest from s2
        scs.push_back(s2[j++]);
    }
    return scs;
}

/*
 * Merges two "close" strings into their shortest common super-sequence (SCS) using banded DP.
 *
 * Parameters:
 *   - s1: First input string.
 *   - s2: Second input string.
 *   - t: Threshold for bandwidth in LCS calculation.
 *
 * Returns:
 *   - The shortest common super-sequence of s1 and s2.
 */
string mergeTwoStrings(const string &s1, const string &s2, int t) {
    vector<vector<int>> dp_vec;
    findLCS(s1, s2, t, dp_vec);
    string lcs = backtrackLCS(s1, s2, dp_vec);
    return buildSCS(s1, s2, lcs);
}

/*
 * Merges a sequence of strings into a single shortest common supersequence (SCS),
 * processing them in a chained manner.
 *
 * Parameters:
 *   - seqs: A vector of strings to merge.
 *   - t: Threshold for band width in LCS calculation.
 *
 * Returns:
 *   - The shortest common supersequence of all input strings.
 */
string mergeAllSeq(const vector<string> &seqs, int t) {
    if (seqs.empty()) {
        return "";
    }
    string result = seqs[0];
    for (int i = 1; i < seqs.size(); i++) { // 0-1  res-2  res-3...
        result = mergeTwoStrings(result, seqs[i], t);
    }
    return result;
}


int main() {
    int k;
    cin >> k;  // Number of sequences in this test case.
    vector<string> seqs(k);
    for (int i = 0; i < k; i++) {
        cin >> seqs[i];
    }

    int t;
    cin >> t;  // Maximum deletions.

    string bestSCS; // Initialize as an empty string

    vector<string> curr_seq = seqs;
    std::sort(curr_seq.begin(), curr_seq.end());

    do {
        string current = mergeAllSeq(curr_seq, t);
        if (bestSCS.empty() || current.size() < bestSCS.size()) {
            bestSCS = current;
        }
    } while (std::next_permutation(curr_seq.begin(), curr_seq.end()));

    cout << "SCS: " << bestSCS << "\n";
    cout << "SCS Length: " << bestSCS.size() << "\n";
    cout << "\n";
    return 0;
}
