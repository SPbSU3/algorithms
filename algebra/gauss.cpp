using ld = long double;
const ld eps = 1e-9;

// Returns vector of solutions of the given linear system.
// Result is empty iff the system is incompatible.
// Otherwise the result equals {(x[i], type[i])}_i, where type[i] = true <=> value of x[i] is fixed.
vector<pair<ld, int>> gauss(vector<vector<ld>> a) {
	vector<pair<ld, bool>> res; 
	
	int n = sz(a);
	if (n == 0) {
		return res;
	}
	int m = sz(a[0]) - 1;
	
	vi where;
	res.resize(m, {0, 0});
	where.resize(m, -1);	
	
	for (int col = 0, row = 0; col < m && row < n; col++) {
		int id = row;
		for (int j = row; j < n; j++)
			if (fabs(a[j][col]) > fabs(a[id][col]))
				id = j;

		if (fabs(a[id][col]) < eps)
			continue;

		for (int j = 0; j <= m; j++)
			swap(a[row][j], a[id][j]);
		where[col] = row;	
		
		for (int j = 0; j < n; j++) {
			if (j != row) {
				ld C = -a[j][col] / a[row][col];
				for (int k = 0; k <= m; k++) {
					a[j][k] += a[row][k] * C;	
				}
			}
		}
			
		row++;
	}
	
	//assigning
	for (int i = 0; i < m; i++) {
		if (where[i] != -1) {
			res[i] = {a[where[i]][m] / a[where[i]][i], 1};
		}
	}
		
	//checking
	for (int i = 0; i < n; i++) {
		ld sum = 0;
		for (int j = 0; j < m; j++) {
			sum += res[j].fst * a[i][j];
		}
		if (fabs(sum - a[i][m]) >= eps) {
			res.clear();
			return res;
		}
	}
	
	return res;
}
