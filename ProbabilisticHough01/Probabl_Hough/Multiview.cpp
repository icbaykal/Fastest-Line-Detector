#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;
using namespace std;

int skew(Mat &S, Mat &S_hat)
{
  S_hat = Mat::zeros(3, 3, CV_64F);
  S_hat.at<double>(0, 1) = -S.at < double >(2);S_hat.at<double>(0, 2) = S.at < double >(1);
  S_hat.at<double>(1, 2) = -S.at < double >(0);
  S_hat.at<double>(1, 0) = S.at < double >(2); S_hat.at<double>(2, 0) = -S.at < double >(1);
  S_hat.at<double>(2, 1) = S.at < double >(0);
  return 0;
}
double triple_product(Mat &a, Mat&b, Mat&c)
{//a,b,c dikey 3lük vektör
  double volume = 0;
  volume = c.at<double>(0, 0)*(a.at<double>(1, 0)*b.at<double>(2, 0) - b.at<double>(1, 0)*a.at<double>(2, 0));
  volume += c.at<double>(1, 0)*(a.at<double>(2, 0)*b.at<double>(0, 0) - b.at<double>(2, 0)*a.at<double>(0, 0));
  volume += c.at<double>(2, 0)*(a.at<double>(0, 0)*b.at<double>(1, 0) - b.at<double>(0, 0)*a.at<double>(1, 0));
  return volume;
}
double exp_rotation(Mat &R, Mat &omega)
{
  double theta = 0;
  theta = R.at<double>(0, 0) + R.at<double>(1, 1) + R.at<double>(1, 1) - 1.0;
  theta = theta / 2.0;

  if (theta < -0.9999) theta = -0.9980;
  if (theta > 0.9999) theta = 0.9980;
  theta = acos(theta);

  if (abs(theta) >= 0.000001)
  {
    omega.at<double>(0, 0) = R.at<double>(2, 1) - R.at<double>(1, 2);
    omega.at<double>(1, 0) = R.at<double>(0, 2) - R.at<double>(2, 0);
    omega.at<double>(2, 0) = R.at<double>(1, 0) - R.at<double>(0, 1);
    omega *= 1.0 / (2.0 * sin(theta));
  }
  else
  {
    omega = (Mat_<double>(3, 1) << 1, 0, 0);
    theta = 0;
  }
  return theta;

}

//Matlab'tan farký biz S1'i kmple yolluyoruz.
Mat rot_matrix(const Mat& omega, double theta)
{
  Mat omega_hat(3, 3, CV_64F);
  static Mat rot_mat(3, 3, CV_64F);
  rot_mat = Mat::eye(3, 3, CV_64F);
  omega_hat = Mat::zeros(3, 3, CV_64F);
  omega_hat.at<double>(0, 1) = -omega.at<double>(1, 0);//Matlab S1(2,1) omega(3)
  omega_hat.at<double>(0, 2) = omega.at<double>(0, 2);//Matlab S1(1,3)omega(2)
  omega_hat.at<double>(1, 0) = omega.at<double>(1, 0);//Matlab S1(2,1)omega(3)
  omega_hat.at<double>(1, 2) = -omega.at<double>(2, 1);//Matlab S1(3,2)omega(1)
  omega_hat.at<double>(2, 0) = -omega.at<double>(0, 2);//Matlab S1(1,3)omega(2)
  omega_hat.at<double>(2, 1) = omega.at<double>(2, 1);//Matlab S1(3,2)omega(1)
  Mat tomega(1, 3, CV_64F);
  tomega.at<double>(0, 0) = omega.at<double>(2, 1);//Matlab S1(3,2)omega(1)
  tomega.at<double>(0, 1) = omega.at<double>(0, 2);//Matlab S1(1,3)omega(2)
  tomega.at<double>(0, 2) = omega.at<double>(1, 0);//Matlab S1(2,1) omega(3)
  double norm_omega = norm(tomega);
  int tnormomega = norm_omega;
  if (tnormomega != 0)
  {
    rot_mat += (omega_hat / norm_omega)* sin(norm_omega*theta);
    rot_mat += (omega_hat*omega_hat) / (norm_omega*norm_omega)*(1.0)*(1 - cos(norm_omega*theta));
  }//Matla else'e gerek yok zaten rot_matrix'te diag([1 1 1] var

   //rot_matrix Matlab ile ayný !!!
   //cout << "rot_mat\n";
   //cout << rot_mat << endl;

  return rot_mat;
}
Mat kron(const Mat& A, const Mat& B)
{
  CV_Assert(A.channels() == 1 && B.channels() == 1);

  Mat1d Ad, Bd;
  A.convertTo(Ad, CV_64F);
  B.convertTo(Bd, CV_64F);

  Mat1d Kd(Ad.rows * Bd.rows, Ad.cols * Bd.cols, 0.0);
  for (int ra = 0; ra < Ad.rows; ++ra)
  {
    for (int ca = 0; ca < Ad.cols; ++ca)
    {
      Kd(Range(ra*Bd.rows, (ra + 1)*Bd.rows), Range(ca*Bd.cols, (ca + 1)*Bd.cols)) = Bd.mul(Ad(ra, ca));
    }
  }
  Mat K;
  Kd.convertTo(K, A.type());
  return K;
}

int Dessential(Mat &p, Mat &q, Mat &T0, Mat &R0)
{
  int i, j, NPOINTS = p.cols;
  if (NPOINTS < 9) return -1;
  Mat A = Mat::zeros(NPOINTS, 9, CV_64F);
  // calculating kroneceker product
  for (i = 0;i < NPOINTS;i++)
  {
    A.at<double>(i, 0) = q.at<double>(0, i)*p.at<double>(0, i);
    A.at<double>(i, 1) = q.at<double>(0, i)*p.at<double>(1, i);
    A.at<double>(i, 2) = q.at<double>(0, i)*p.at<double>(2, i);
    A.at<double>(i, 3) = q.at<double>(1, i)*p.at<double>(0, i);
    A.at<double>(i, 4) = q.at<double>(1, i)*p.at<double>(1, i);
    A.at<double>(i, 5) = q.at<double>(1, i)*p.at<double>(2, i);
    A.at<double>(i, 6) = q.at<double>(2, i)*p.at<double>(0, i);
    A.at<double>(i, 7) = q.at<double>(2, i)*p.at<double>(1, i);
    A.at<double>(i, 8) = q.at<double>(2, i)*p.at<double>(2, i);
  }
  Mat U, S, V;
  SVD::compute(A, S, U, V, SVD::FULL_UV);
  // pick the eigenvector corresponding to the smallest eigenvalue
  Mat eigs98(1, 2, CV_64F);eigs98.at<double>(0, 0) = S.at<double>(8);
  eigs98.at<double>(0, 1) = S.at<double>(7);
  //eigs98 Matlab ile ayný

  Mat e(1, 9, CV_64F);
  e = V.row(8).t();
  Mat E(3, 3, CV_64F);
  E.row(0) = e.rowRange(0, 3).t();
  E.row(1) = e.rowRange(3, 6).t();
  E.row(2) = e.rowRange(6, 9).t();
  E = -E;// Matlab ile ayný þu anda


  Mat Rzp = (Mat_<double>(3, 3) << 0, -1, 0, 1, 0, 0, 0, 0, 1);
  Mat Rzn = (Mat_<double>(3, 3) << 0, 1, 0, -1, 0, 0, 0, 0, 1);
  Mat S1;
  SVD::compute(E, S, U, V, SVD::FULL_UV);//S,U(-),V.t ayný Matlab ile
  V = V.t();
  U = -U;
  S = Mat::eye(3, 3, CV_64F);
  S.at<double>(2, 2) = 0;
  double detu = determinant(U);//Matlab -detu
  double detv = determinant(V);//Matlab detv
  if (detu < 0.0 && detv < 0.0)
  {
    U = -U; V = -V;
  }
  else if (detu < 0.0 && detv > 0.0)
  {
    S1 = Rzp*S;
    U = -U*rot_matrix(S1, 3.1415)*Rzp;
    V = V*Rzp;
  }
  else if (detu < 0.0 && detv > 0.0)
  {
    S1 = Rzp*S;
    U = U*rot_matrix(S1, 3.1415)*Rzp;
    V = -V*Rzp;
  }

  Mat R[4], Th[4];
  R[0] = U*Rzp.t()*V.t();
  Th[0] = U*Rzp*S*U.t();
  Th[0] = -Th[0];//!!!!!!!!!!!!!
  Mat t(3, 4, CV_64F);
  t.at<double>(0, 0) = -Th[0].at<double>(1, 2);    t.at<double>(1, 0) = Th[0].at<double>(0, 2);
  t.at<double>(2, 0) = -Th[0].at<double>(0, 1);
  //MATLAB [omega(:, 1), theta(1)] = exp_rotation(R(:, : , 1));
  Mat omega1(3, 1, CV_64F);
  double theta = exp_rotation(R[0], omega1);

  R[1] = U*Rzn.t()*V.t();
  Th[1] = U*Rzn*S*U.t();
  Th[1] = -Th[1];//!!!!!!!!!!!!!!!!!
  t.at<double>(0, 1) = -Th[1].at<double>(1, 2);    t.at<double>(1, 1) = Th[1].at<double>(0, 2);
  t.at<double>(2, 1) = -Th[1].at<double>(0, 1);
  Mat omega2(3, 1, CV_64F);
  theta = exp_rotation(R[1], omega2);
  //%-----------------------------------------
  SVD::compute(-E, S, U, V, SVD::FULL_UV);
  V = V.t();
  U = -U;
  S = Mat::eye(3, 3, CV_64F);
  S.at<double>(2, 2) = 0;
  detu = determinant(U);//Matlab -detu
  detv = determinant(V);//Matlab detv
  if (detu < 0.0 && detv < 0.0)
  {
    U = -U; V = -V;
  }
  else if (detu < 0.0 && detv > 0.0)
  {
    S1 = Rzp*S;
    U = -U*rot_matrix(S1, 3.1415)*Rzp;
    V = V*Rzp;
  }
  else if (detu < 0.0 && detv > 0.0)
  {
    S1 = Rzp*S;
    U = U*rot_matrix(S1, 3.1415)*Rzp;
    V = -V*Rzp;
  }

  R[2] = U*Rzp.t()*V.t();
  Th[2] = U*Rzp*S*U.t();
  Th[2] = -Th[2];//!!!!!!!!!!!!!
  t.at<double>(0, 2) = -Th[2].at<double>(1, 2);    t.at<double>(1, 2) = Th[2].at<double>(0, 2);
  t.at<double>(2, 2) = -Th[2].at<double>(0, 1);
  Mat omega3(3, 1, CV_64F);
  theta = exp_rotation(R[2], omega3);

  R[3] = U*Rzn.t()*V.t();
  Th[3] = U*Rzn*S*U.t();
  Th[3] = -Th[3];//!!!!!!!!!!!!!!!!!
  t.at<double>(0, 3) = -Th[3].at<double>(1, 2);    t.at<double>(1, 3) = Th[3].at<double>(0, 2);
  t.at<double>(2, 3) = -Th[3].at<double>(0, 1);
  Mat omega4(3, 1, CV_64F);
  theta = exp_rotation(R[3], omega4);

  int index;
  Mat posdepth = Mat::zeros(1, 4, CV_64F);
  Mat rp, thq;
  Mat S_hat(3, 3, CV_64F), qt;
  Mat alpha1 = Mat::zeros(NPOINTS, 1, CV_64F);

  Mat volume = Mat::zeros(NPOINTS, 1, CV_64F);
  Mat postdepth = Mat::zeros(4, 1, CV_64F);
  double vol, depth, normy;
  for (i = 0;i < 4;i++)
  {
    for (j = 0;j < NPOINTS;j++)
    {
      rp = R[i] * p.col(j);//R ve pnin çarpýmý
      thq = Th[i] * q.col(j);//Th ve q'nun çarpýmý
                             //volume(j) = triple_product(t(:, i), R(:, : , i)*p(:, j), Th(:, : , i)*q(:, j));
      volume.at<double>(j) = triple_product(t.col(i), rp, thq);
      skew(q.col(j), S_hat);
      //rp temp matrix: skew(q(:,j))*t(:,i))'
      rp = S_hat*t.col(i);
      rp = rp.t();
      //thq temp matrix: (skew(q(:,j))*R(:,:,i)*p(:,j))
      thq = S_hat*R[i] * p.col(j);
      rp = rp*thq;
      //thq: skew(q(:,j))*t(:,i))
      thq = S_hat*t.col(i);
      normy = norm(thq);
      alpha1.at<double>(j) = -rp.at<double>(0) / (normy*normy);

    }
    // MATLAB: vol = sum(sign(volume));
    vol = 0;depth = 0;
    for (j = 0;j < NPOINTS;j++)
    {
      if (volume.at<double>(j)>0.00000)vol++;
      else if (volume.at<double>(j)<0.00000)vol--;

      if (alpha1.at<double>(j)>0.00000)depth++;
      else if (alpha1.at<double>(j)<0.00000)depth--;
    }
    postdepth.at<double>(i) = vol + depth;
    //cout << "volume: ";
    //cout << volume << endl;

  }
  double min, max;
  cv::Point min_loc, max_loc;
  cv::minMaxLoc(postdepth, &min, &max, &min_loc, &max_loc);
  T0 = t.col(max_loc.y);
  R0 = R[max_loc.y];
  return 0;
}

//Bu fonksiyon Matlab'takinin eþi
int compute3DStructure(Mat &p, Mat &q, Mat &R, Mat &T, Mat &XP)
{
  int i, nc = q.cols;
  Mat A(4, 4, CV_64F);
  Mat X(4, nc, CV_64F);
  Mat t;//temporary mat
  Mat ua, sa, va;//svd matrices
  for (i = 0;i <nc;i++)
  {
    A.at<double>(0, 0) = 0;  A.at<double>(0, 1) = -1;  A.at<double>(0, 2) = p.at<double>(1, i);  A.at<double>(0, 3) = 0;
    A.at<double>(1, 0) = -1;  A.at<double>(1, 1) = 0;  A.at<double>(1, 2) = p.at<double>(0, i);  A.at<double>(1, 3) = 0;
    t = q.at<double>(1, i)*R.rowRange(2, 3) - R.rowRange(1, 2);
    A.at<double>(2, 0) = t.at<double>(0);A.at<double>(2, 1) = t.at<double>(1);A.at<double>(2, 2) = t.at<double>(2);
    A.at<double>(2, 3) = q.at<double>(1, i)*T.at<double>(2) - T.at<double>(1);
    t = q.at<double>(0, i)*R.rowRange(2, 3) - R.rowRange(0, 1);
    A.at<double>(3, 0) = t.at<double>(0);A.at<double>(3, 1) = t.at<double>(1);A.at<double>(3, 2) = t.at<double>(2);
    A.at<double>(3, 3) = q.at<double>(0, i)*T.at<double>(2) - T.at<double>(0);
    SVD::compute(A, sa, ua, va);//bizim va matlabdakinin traversi
    X.at<double>(0, i) = -va.at<double>(3, 0);X.at<double>(1, i) = -va.at<double>(3, 1);
    X.at<double>(2, i) = -va.at<double>(3, 2);X.at<double>(3, i) = -va.at<double>(3, 3);
  }
  XP = Mat::zeros(3, nc, CV_64F);
  divide(X.row(0), X.row(3), XP.row(0));
  divide(X.row(1), X.row(3), XP.row(1));
  divide(X.row(2), X.row(3), XP.row(2));
  return 0;
}

int Multiview(Mat &Khat, Mat &xn1, Mat &xn2, Mat &Reslt)
{
  int NPOINTS = xn1.cols;
  vector<Point2d>points1(NPOINTS);
  vector<Point2d>points2(NPOINTS);
  xn1 = Khat.inv()*xn1;
  xn2 = Khat.inv()*xn2;
  for (int i = 0;i < NPOINTS;i++)
  {
    points1[i].x = xn1.at<double>(0, i);
    points1[i].y = xn1.at<double>(1, i);
    points2[i].x = xn2.at<double>(0, i);
    points2[i].y = xn2.at<double>(1, i);
  }
  //Matlab dessential
  Mat T0(3, 1, CV_64F);
  Mat R0(3, 3, CV_64F);
  Dessential(xn1, xn2, T0, R0);
  compute3DStructure(xn1, xn2, R0, T0, Reslt);
  return 0;
}