% stats.sl
%
% Statistical data analysis functions for use with slash.  All data
% are supplied as matrices, analyses are performed column-wise.
% Most of the behaviour mimics mat/sci-lab.
%
% References:
%	Isaaks, E.H. and Srivastava, R.M., 1989, Applied Geostatistics
%	Press et al., 1992, Numerical Recipes in C
%	Spiegel, M.R., 1972, Theory and Problems of Statistics.

%I:stats
%@ Void stats
% Initialize statistical data analysis library. None of the
% functions in @stats.sl@ can be used before this command is
% issued.
%
% All statistical data analysis is performed on matrices (2D
% floating point arrays) in which the data are column-wise
% organized.
define stats;

% Univariate Analyses

% function used by median() to sort array
define sort_numcmp(a, b)
{
	return(a < b);
}

%I:median
%@ Array median(Array mtx)
% Returns a row vector with the median values for each column
% in @mtx@.
%
define median(mtx)
{
	variable i, j, m, n, ind, ind2, tmp, vec, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);
	tmp = create_array('f', m, 1);

	_for(0, n - 1, 1) {
		=j;
		_for(0, m - 1, 1) {
			=i;
			tmp[i] = mtx[i,j];
		}
		vec = array_sort(tmp, "sort_numcmp");
		if(m & 1) {
			ind = vec[m / 2];
			res[0,j] = mtx[ind,j];
		}
		else {
			ind2 = vec[m / 2];
			ind = vec[m / 2 - 1];
			res[0,j] = (mtx[ind,j] + mtx[ind2,j]) / 2;
		}
	}
	return(res);
}

%I:mean
%@ Array mean(Array mtx)
% Returns a row vector with the mean values for each column
% in @mtx@.
%
define mean(mtx)
{
	variable i, j, m, n, sum, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);
	_for(0, n - 1, 1) {
		=j;
		sum = 0.0;
		_for(0, m - 1, 1) {
			=i;
			sum = sum + mtx[i,j];
		}
		res[0,j] = sum / m;
	}
	return(res);
}

%I:rmsq
%@ Array rmsq(Array mtx)
% Returns a row vector with the root mean square or quadratic
% mean values for each column in @mtx@.
%
define rmsq(mtx)
{
	variable i, j, m, n, sum2, x, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

	_for(0, n - 1, 1) {
		=j;
		sum2 = 0.0;
		_for(0, m - 1, 1) {
			=i;
			x = mtx[i,j];
			sum2 = sum2 + sqr(x);
		}
		res[0,j] = sqrt(sum2 / m);
	}
	return(res);
}

%I:mmax
%@ Array mmax(Array mtx)
% Returns a row vector with the maximum values for each column
% in @mtx@.
%
define mmax(mtx)
{
	variable i, j, m, n, tmp, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

	_for(0, n - 1, 1) {
		=j;
		tmp = mtx[0,j];
		_for(0, m - 1, 1) {
			=i;
			if(mtx[i,j] > tmp)
				tmp = mtx[i,j];
		}
		res[0,j] = tmp;
	}
	return(res);
}

%I:mmin
%@ Array mmin(Array mtx)
% Returns a row vector with the minimum values for each column
% in @mtx@.
%
define mmin(mtx)
{
	variable i, j, m, n, tmp, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

	_for(0, n - 1, 1) {
		=j;
		tmp = mtx[0,j];
		_for(0, m - 1, 1) {
			=i;
			if(mtx[i,j] < tmp)
				tmp = mtx[i,j];
		}
		res[0,j] = tmp;
	}
	return(res);
}

%I:adev
%@ Array adev(Array mtx)
% Returns a row vector with the absolute mean deviations for
% each column in @mtx@.
%
define adev(mtx)
{
	variable i, j, m ,n, sum, avg, diff, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

	_for(0, n - 1, 1) {
		=j;
		sum = 0.0;
		_for(0, m - 1, 1) {
			=i;
			sum = sum + mtx[i,j];
		}
		avg = sum / m;
		sum = 0.0;
		_for(0, m - 1, 1) {
			=i;
			diff = mtx[i,j] - avg;
			if(diff < 0.0)
				sum = sum - diff;
			else
				sum = sum + diff;
		}
		res[0,j] = sum / m;
	}
	return(res);
}

%I:sdev
%@ Array sdev(Array mtx)
% Returns a row vector with the standard deviations for each
% column in @mtx@.
%
define sdev(mtx)
{
% Uses corrected two-pass algorithm (cf. PRESS92, Eqn 14.1.8)
	variable i, j, m, n, avg, diff, sum, sum2, var, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

	_for(0, n - 1, 1) {
	% first pass
		=j;
		sum = 0.0;
		_for(0, m - 1, 1) {
			=i;
			sum = sum + mtx[i,j];
		}
		avg = sum / m;
	% second pass
		sum = 0.0;
		sum2 = 0.0;
		_for(0, m - 1, 1) {
			=i;
			diff = mtx[i,j] - avg;
			sum = sum + diff;
			sum2 = sum2 + sqr(diff);
		}
	% arbitrary sample size to use (N - 1)
	%	if(m > 32)
	%		var = (sum2 - sqr(sum) / m) / (m - 1);
	%	else
			var = (sum2 - sqr(sum) / m) / m;
		res[0,j] = sqrt(var);
	}
	return(res);
}

% Bivariate Analyses %

%I:linreg
%@ Array linreg(Array mtx)
% Calculates linear regression coefficients for straight line
% fitting, i.e: y = Ax + B. A is slope and B is y-intercept.
% Argument is minimum 2*2 matrix, regression is performed for
% all @mtx@ columns j>=1 (y-data) against column 0 (x-data).
%
% Returns a 2*n matrix in which for each column j in @mtx@,
% Array[0,j] = A and Array[1,j] = B. Hence, Array[0,0] = 1 and
% Array[1,0] = 0.
%
define linreg(mtx)
{
	variable i, j, m, n, x, y, sumx, sumy, sumx2, sumxy, frx, res;

	(m, n) = asize(mtx);
	res = matrix(2, n);

% first, calculate x-data
	sumx = 0.0;
	sumx2 = 0.0;
	_for(0, m - 1, 1) {
		=i;
		x = mtx[i,0];
		sumx = sumx + x;
		sumx2 = sumx2 + sqr(x);
	}
	frx = m * sumx2 - sqr(sumx);
	res[0,0] = 1;

% now, handle y-data columns
	_for(1, n - 1, 1) {
		=j;
		sumy = 0.0;
		sumxy = 0.0;
		_for(0, m - 1, 1) {
			=i;
			y = mtx[i,j];
			sumy = sumy + y;
			sumxy = sumxy + mtx[i,0] * y;
		}
		res[0,j] = (m * sumxy - sumx * sumy) / frx;
		res[1,j] = (sumy * sumx2 - sumx * sumxy) / frx;
	}
	return(res);
}

%I:corr
%@ Array corr(Array mtx)
% Calculate linear correlation coefficient (r) between x-data in
% column 0 and y-data in subsequent columns.
%
% Returns a row vector with correlation coefficients for each
% column in @mtx@ w.r.t. column 0. Hence, Array[0,0] = 1.
%
define corr(mtx)
{
	variable i, j, m, n, x, y;
	variable sumx, sumy, sumx2, sumy2, sumxy, frx, fry, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

% first, calculate x-data
	sumx = 0.0;
	sumx2 = 0.0;
	_for(0, m - 1, 1) {
		=i;
		x = mtx[i,0];
		sumx = sumx + x;
		sumx2 = sumx2 + sqr(x);
	}
	frx = m * sumx2 - sqr(sumx);
	res[0,0] = 1;

% now, handle y-data columns
	_for(1, n - 1, 1) {
		=j;
		sumy = 0.0;
		sumy2 = 0.0;
		sumxy = 0.0;
		_for(0, m - 1, 1) {
			=i;
			y = mtx[i,j];
			sumy = sumy + y;
			sumy2 = sumy2 + sqr(y);
			sumxy = sumxy + mtx[i,0] * y;
		}
		fry = m * sumy2 - sqr(sumy);
		res[0,j] = (m * sumxy - sumx * sumy) / sqrt(frx * fry);
	}
	return(res);
}

%I:covar
%@ Array covar(Array mtx)
% Calculates the covariance between x-data in @mtx@ column 0 and
% y-data in subsequent columns.
%
% Returns a row vector with covariances for each column in @mtx@
% w.r.t. column 0. Hence, Array[0,0] contains the variance of the
% data in the first column.
%
define covar(mtx)
{
	variable i, j, m, n, mu, dxy, res;

	(m, n) = asize(mtx);
	res = matrix(1, n);

% first, calculate means
	mu = mean(mtx);

% now, handle y-data columns
	_for(0, n - 1, 1) {
		=j;
		dxy = 0.0;
		_for(0, m - 1, 1) {
			=i;
			dxy = dxy + (mtx[i,0] - mu[0,0]) * (mtx[i,j] - mu[0,j]);
		}
		res[0,j] = dxy / m;
	}
	return(res);
}
