#pragma once
#include <complexities/complexity_analyzer.h>
#include <random>
#include <vector>

namespace ds::utils
{
	template <typename T>
	class Matrix
	{
	public:
		virtual ~Matrix() = default;
		virtual T& at(size_t row, size_t col) = 0;
		virtual const T& at(size_t row, size_t col) const = 0; // Add this line
		virtual void set(size_t row, size_t col, T value) = 0;
		virtual double determinant() = 0;
		virtual void multiplyBy(const Matrix<T>& other) = 0;

		virtual size_t rows() const = 0;
		virtual size_t cols() const = 0;
	};

	template<typename T >
	class ContinuousMatrix : public Matrix<T>
	{
	private:
		std::vector<T> data_;
		size_t rows_;
		size_t cols_;
	public:
		ContinuousMatrix() : rows_(0), cols_(0) {}
		ContinuousMatrix(size_t rows, size_t cols) :
			rows_(rows), cols_(cols), data_(rows* cols) {
		}
		T& at(size_t row, size_t col) override {
			return data_[row * cols_ + col];
		}
		const T& at(size_t row, size_t col) const override{
			return data_[row * cols_ + col];
		}

		void set(size_t row, size_t col, T value) override {
			data_[row * cols_ + col] = value;
		}

		double determinant() override {
			if (rows_ != cols_) return 0;
			if (rows_ == 2) {
				return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);
			}
			else if (rows_ == 3) {
				return at(0, 0) * (at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1)) -
					at(0, 1) * (at(1, 0) * at(2, 2) - at(1, 2) * at(2, 0)) +
					at(0, 2) * (at(1, 0) * at(2, 1) - at(1, 1) * at(2, 0));
			}
			return 0;
		}
		void multiplyBy(const Matrix<T>& other) override {
			if (cols_ != other.rows()) {
				throw std::invalid_argument("Matrix dimensions do not match for multiplication");
			}
			ContinuousMatrix<T> result(rows_, other.cols());

			for (size_t i = 0; i < rows_; ++i) {
				for (size_t j = 0; j < other.cols(); ++j) {
					T sum = 0;
					for (size_t k = 0; k < cols_; ++k) {
						sum += at(i, k) * dynamic_cast<const ContinuousMatrix<T>&>(other).at(k, j);
					}
					result.set(i, j, sum);
				}
			}
			*this = std::move(result);
		}

		size_t rows() const override { return rows_; }
		size_t cols() const override { return cols_; }
	};
	// dynamicaly alocated field of fields
	template<typename T>
	class PointerMatrix : public Matrix<T>
	{
	private:
		std::vector<std::vector<T>> data_;
		size_t rows_;
		size_t cols_;
	public:
		PointerMatrix() : rows_(0), cols_(0) {}
		PointerMatrix(size_t rows, size_t cols) :
			rows_(rows), cols_(cols), data_(rows)
		{
			for (auto& row : data_)
			{
				row.resize(cols);
			}
		}

		T& at(size_t row, size_t col) override
		{
			return data_[row][col];
		}

		const T& at(size_t row, size_t col) const override
		{
			return data_[row][col];
		}

		void set(size_t row, size_t col, T value) override
		{
			data_[row][col] = value;
		}

		double determinant() override
		{
			if (rows_ != cols_) return 0;
			if (rows_ == 2)
			{
				return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);
			}
			else if (rows_ == 3)
			{
				return at(0, 0) * (at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1)) -
					at(0, 1) * (at(1, 0) * at(2, 2) - at(1, 2) * at(2, 0)) +
					at(0, 2) * (at(1, 0) * at(2, 1) - at(1, 1) * at(2, 0));
			}
			return 0;
		}
		void multiplyBy(const Matrix<T>& other) override
		{
			if (cols_ != other.rows())
			{
				throw std::invalid_argument("Matrix dimensions do not match for multiplication");
			}

			PointerMatrix<T> result(rows_, other.cols());

			for (size_t i = 0; i < rows_; ++i)
			{
				for (size_t j = 0; j < other.cols(); ++j)
				{
					T sum = 0;
					for (size_t k = 0; k < cols_; ++k)
					{
						sum += at(i, k) * dynamic_cast<const PointerMatrix<T>&>(other).at(k, j);
					}
					result.set(i, j, sum);
				}
			}
			*this = std::move(result);
		}
		size_t rows() const override { return rows_; }
		size_t cols() const override { return cols_; }
	};

	/**
	 *@brief basic class for matrix analyzer
	 */
	template<class MatrixType>
	class MatrixAnalyzer : public ComplexityAnalyzer<MatrixType>
	{
	protected:
		explicit MatrixAnalyzer(const std::string& name) :
			ComplexityAnalyzer<MatrixType>(name),
			rng_(144)
		{
		}

	protected:
		void growToSize(MatrixType& matrix, size_t size) override
		{

		}
		int getRandomValue() const
		{
			return rng_() % 100;
		}
	private:
		mutable std::default_random_engine rng_;
	};
	/**
	 *@brief analyses the time complexity when accessing item from matrix
	 */
	template<class MatrixType>
	class MatrixAccessAnalyzer : public MatrixAnalyzer<MatrixType>
	{
	public:
		explicit MatrixAccessAnalyzer(const std::string& name) :
			MatrixAnalyzer<MatrixType>(name) {
		}

	protected:
		void growToSize(MatrixType& matrix, size_t size) override {
			matrix = MatrixType(size, size);

			//fill the matrix with random values
			for (size_t i = 0; i < size; ++i) {
				for (size_t j = 0; j < size; ++j) {
					matrix.set(i, j, this->getRandomValue());
				}
			}

		}

		void executeOperation(MatrixType& matrix) override {
			//access random item from the matrix
			size_t i = std::rand() % matrix.rows();
			size_t j = std::rand() % matrix.cols();
			volatile auto tmp = matrix.at(i, j);
			(void)tmp; // prevent optimization
		}

	};
	/**
	 *@brief analzese the time complexity when calculating the determinant
	 */
	template<class MatrixType>
	class MatrixDeterminantAnalyzer : public MatrixAnalyzer<MatrixType>
	{
	public:
		explicit MatrixDeterminantAnalyzer(const std::string& name) :
			MatrixAnalyzer<MatrixType>(name) {
		}
	protected:
		void growToSize(MatrixType& matrix, size_t size) override {
			size = std::min<size_t>(size, 10);

			matrix = MatrixType(size, size);

			for (size_t i = 0; i < size; ++i) {
				for (size_t j = 0; j < size; ++j) {
					matrix.set(i, j, this->getRandomValue());
				}
			}
		}

		void executeOperation(MatrixType& matrix) override {
			volatile auto det = matrix.determinant();
			(void)det; // prevent optimization
		}

	};
	/**
	 *@brief analzese the time complexity when multiplying two matrices
	 */

	template<class MatrixType >
	class MatrixMultiplicationAnalyzer : public MatrixAnalyzer<MatrixType>
	{
	public:
		explicit MatrixMultiplicationAnalyzer(const std::string& name) :
			MatrixAnalyzer<MatrixType>(name),
			secondMatrix_(nullptr) {
		}
		~MatrixMultiplicationAnalyzer()
		{
			delete secondMatrix_;
		}
	protected:
		void growToSize(MatrixType& matrix, size_t size) override {
			//Constraint the size of the matrix to 100x100
			size = std::min<size_t>(size, 100);
			matrix = MatrixType(size, size);

			//creation of second matrix
			if (secondMatrix_) delete secondMatrix_;
			secondMatrix_ = new MatrixType(size, size);

			// fill the matrix with random values
			for (size_t i = 0; i < size; ++i) {
				for (size_t j = 0; j < size; ++j) {
					matrix.set(i, j, this->getRandomValue());
					secondMatrix_->set(i, j, this->getRandomValue());
				}
			}
		}

		void executeOperation(MatrixType& matrix) override {
			//multiply the matrix with the second matrix
			matrix.multiplyBy(*secondMatrix_);
		}



	private:
		MatrixType* secondMatrix_;
	};

	/**
	 *@brief container for all matrix analyzers
	 */

	class MatrixAnalyzerContainer : public CompositeAnalyzer {
	public:
		MatrixAnalyzerContainer() :
			CompositeAnalyzer("matrix-analyzer") {

			//ContinuousMatrix
			this->addAnalyzer(std::make_unique<MatrixAccessAnalyzer<ContinuousMatrix<int>>>("continuous-matrix-access"));
			this->addAnalyzer(std::make_unique<MatrixDeterminantAnalyzer<ContinuousMatrix<int>>>("continuous-matrix-determinant"));
			this->addAnalyzer(std::make_unique<MatrixMultiplicationAnalyzer<ContinuousMatrix<int>>>("continuous-matrix-multiplication"));

			//PointerMatrix
			this->addAnalyzer(std::make_unique<MatrixAccessAnalyzer<PointerMatrix<int>>>("pointer-matrix-access"));
			this->addAnalyzer(std::make_unique<MatrixDeterminantAnalyzer<PointerMatrix<int>>>("pointer-matrix-determinant"));
			this->addAnalyzer(std::make_unique<MatrixMultiplicationAnalyzer<PointerMatrix<int>>>("pointer-matrix-multiplication"));
		}
	};

};

