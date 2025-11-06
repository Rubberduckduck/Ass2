#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <chrono>
#include <string>

using namespace cv;

class SeamCarver {
private:
    cv::Mat image;
    
    // Calculate energy map using gradient magnitude
    cv::Mat calculateEnergyMap(const cv::Mat& img) {
        cv::Mat gray, gradX, gradY, absGradX, absGradY, energy;
        
        // Convert to grayscale if needed
        if (img.channels() == 3) {
            cvtColor(img, gray, COLOR_BGR2GRAY);
        } else {
            gray = img.clone();
        }
        
        // Apply Sobel filter for gradient computation
        Sobel(gray, gradX, CV_64F, 1, 0, 3);
        Sobel(gray, gradY, CV_64F, 0, 1, 3);
        
        // Convert to absolute values
        convertScaleAbs(gradX, absGradX);
        convertScaleAbs(gradY, absGradY);
        
        // Compute energy as gradient magnitude
        addWeighted(absGradX, 0.5, absGradY, 0.5, 0, energy);
        energy.convertTo(energy, CV_64F);
        
        return energy;
    }
    
    // Find vertical seam using Dynamic Programming
    std::vector<int> findVerticalSeamDP(const cv::Mat& energy) {
        int rows = energy.rows;
        int cols = energy.cols;
        
        // DP table - we only need current and previous row
        std::vector<double> prev(cols), curr(cols);
        std::vector<std::vector<int>> parent(rows, std::vector<int>(cols));
        
        // Initialize first row
        for (int j = 0; j < cols; j++) {
            prev[j] = energy.at<double>(0, j);
        }
        
        // Fill DP table
        for (int i = 1; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // Center, left, right
                const int offsets[] = {-1, 0, 1}; 
                double minEnergy = std::numeric_limits<double>::max();
                int minIdx = j;
                for (int d : offsets) {
                    int prevCol = j + d;
                    if (prevCol >= 0 && prevCol < cols) {
                        if (prev[prevCol] < minEnergy) {
                            minEnergy = prev[prevCol];
                            minIdx = prevCol;
                        }
                    }
                }
                curr[j] = energy.at<double>(i, j) + minEnergy;
                parent[i][j] = minIdx;
            }
            std::swap(prev, curr);
        }

        
        // Find minimum in last row
        int minCol = 0;
        double minEnergy = prev[0];
        for (int j = 1; j < cols; j++) {
            if (prev[j] < minEnergy) {
                minEnergy = prev[j];
                minCol = j;
            }
        }
        
        // Backtrack to find seam
        std::vector<int> seam(rows);
        seam[rows-1] = minCol;
        for (int i = rows-2; i >= 0; i--) {
            seam[i] = parent[i+1][seam[i+1]];
        }
        
        return seam;
    }
    
    // Find vertical seam using Greedy Algorithm
    std::vector<int> findVerticalSeamGreedy(const cv::Mat& energy) {
        int rows = energy.rows;
        int cols = energy.cols;
        std::vector<int> seam(rows);
        
        // Start from minimum energy in first row
        double minEnergy = energy.at<double>(0, 0);
        int minCol = 0;
        for (int j = 1; j < cols; j++) {
            if (energy.at<double>(0, j) < minEnergy) {
                minEnergy = energy.at<double>(0, j);
                minCol = j;
            }
        }
        seam[0] = minCol;
        
        // Greedily choose minimum energy neighbor in each row
        for (int i = 1; i < rows; i++) {
            int prevCol = seam[i-1];
            minEnergy = energy.at<double>(i, prevCol);
            minCol = prevCol;
            
            // Check left
            if (prevCol > 0 && energy.at<double>(i, prevCol-1) < minEnergy) {
                minEnergy = energy.at<double>(i, prevCol-1);
                minCol = prevCol-1;
            }
            
            // Check right
            if (prevCol < cols-1 && energy.at<double>(i, prevCol+1) < minEnergy) {
                minEnergy = energy.at<double>(i, prevCol+1);
                minCol = prevCol+1;
            }
            
            seam[i] = minCol;
        }
        
        return seam;
    }
    
    // Remove vertical seam from image
    cv::Mat removeVerticalSeam(const cv::Mat& img, const std::vector<int>& seam) {
        int rows = img.rows;
        int cols = img.cols;
        cv::Mat result(rows, cols-1, img.type());
        
        for (int i = 0; i < rows; i++) {
            int seamCol = seam[i];
            
            if (img.channels() == 3) {
                for (int j = 0; j < seamCol; j++) {
                    result.at<Vec3b>(i, j) = img.at<Vec3b>(i, j);
                }
                for (int j = seamCol+1; j < cols; j++) {
                    result.at<Vec3b>(i, j-1) = img.at<Vec3b>(i, j);
                }
            } else {
                for (int j = 0; j < seamCol; j++) {
                    result.at<uchar>(i, j) = img.at<uchar>(i, j);
                }
                for (int j = seamCol+1; j < cols; j++) {
                    result.at<uchar>(i, j-1) = img.at<uchar>(i, j);
                }
            }
        }
        
        return result;
    }
    
    // Visualize seam on image
    cv::Mat visualizeSeam(const cv::Mat& img, const std::vector<int>& seam, bool isVertical = true) {
        cv::Mat vis = img.clone();
        
        if (isVertical) {
            for (int i = 0; i < seam.size(); i++) {
                vis.at<Vec3b>(i, seam[i]) = Vec3b(0, 0, 255); // Red
            }
        } else {
            for (int j = 0; j < seam.size(); j++) {
                vis.at<Vec3b>(seam[j], j) = Vec3b(0, 0, 255); // Red
            }
        }
        
        return vis;
    }
    
    // Find horizontal seam (transpose, find vertical, transpose back)
    std::vector<int> findHorizontalSeamDP(const cv::Mat& energy) {
        cv::Mat transposed;
        transpose(energy, transposed);
        return findVerticalSeamDP(transposed);
    }
    
    std::vector<int> findHorizontalSeamGreedy(const cv::Mat& energy) {
        cv::Mat transposed;
        transpose(energy, transposed);
        return findVerticalSeamGreedy(transposed);
    }
    
    // Remove horizontal seam
    cv::Mat removeHorizontalSeam(const cv::Mat& img, const std::vector<int>& seam) {
        cv::Mat transposed, result;
        transpose(img, transposed);
        result = removeVerticalSeam(transposed, seam);
        transpose(result, result);
        return result;
    }

public:
    SeamCarver(const cv::Mat& img) : image(img.clone()) {}
    
    // Resize image by removing vertical seams using DP
    cv::Mat resizeVerticalDP(int targetWidth, bool visualize = false) {
        cv::Mat result = image.clone();
        int seamsToRemove = image.cols - targetWidth;
        
        std::cout << "Removing " << seamsToRemove << " vertical seams using DP..." << std::endl;
        
        for (int i = 0; i < seamsToRemove; i++) {
            cv::Mat energy = calculateEnergyMap(result);
            std::vector<int> seam = findVerticalSeamDP(energy);
            
            if (visualize && i % (seamsToRemove/10 + 1) == 0) {
                cv::Mat vis = visualizeSeam(result, seam, true);
                imshow("Seam Visualization", vis);
                waitKey(100);
            }
            
            result = removeVerticalSeam(result, seam);
            
            if ((i+1) % 10 == 0 || i == seamsToRemove-1) {
                std::cout << "Progress: " << (i+1) << "/" << seamsToRemove << std::endl;
            }
        }
        
        return result;
    }
    
    // Resize image by removing vertical seams using Greedy
    cv::Mat resizeVerticalGreedy(int targetWidth) {
        cv::Mat result = image.clone();
        int seamsToRemove = image.cols - targetWidth;
        
        std::cout << "Removing " << seamsToRemove << " vertical seams using Greedy..." << std::endl;
        
        for (int i = 0; i < seamsToRemove; i++) {
            cv::Mat energy = calculateEnergyMap(result);
            std::vector<int> seam = findVerticalSeamGreedy(energy);
            result = removeVerticalSeam(result, seam);
            
            if ((i+1) % 10 == 0 || i == seamsToRemove-1) {
                std::cout << "Progress: " << (i+1) << "/" << seamsToRemove << std::endl;
            }
        }
        
        return result;
    }
    
    // Resize image by removing horizontal seams using DP
    cv::Mat resizeHorizontalDP(int targetHeight, bool visualize = false) {
        cv::Mat result = image.clone();
        int seamsToRemove = image.rows - targetHeight;
        
        std::cout << "Removing " << seamsToRemove << " horizontal seams using DP..." << std::endl;
        
        for (int i = 0; i < seamsToRemove; i++) {
            cv::Mat energy = calculateEnergyMap(result);
            std::vector<int> seam = findHorizontalSeamDP(energy);
            
            if (visualize && i % (seamsToRemove/10 + 1) == 0) {
                cv::Mat vis = visualizeSeam(result, seam, false);
                imshow("Seam Visualization", vis);
                waitKey(100);
            }
            
            result = removeHorizontalSeam(result, seam);
            
            if ((i+1) % 10 == 0 || i == seamsToRemove-1) {
                std::cout << "Progress: " << (i+1) << "/" << seamsToRemove << std::endl;
            }
        }
        
        return result;
    }
    
    // Resize both dimensions
    cv::Mat resize(int targetWidth, int targetHeight, bool useDP = true, bool visualize = false) {
        cv::Mat result = image.clone();
        
        // Remove vertical seams first
        if (targetWidth < image.cols) {
            SeamCarver carver(result);
            if (useDP) {
                result = carver.resizeVerticalDP(targetWidth, visualize);
            } else {
                result = carver.resizeVerticalGreedy(targetWidth);
            }
        }
        
        // Then remove horizontal seams
        if (targetHeight < image.rows) {
            SeamCarver carver(result);
            if (useDP) {
                result = carver.resizeHorizontalDP(targetHeight, visualize);
            } else {
                // Implement greedy for horizontal if needed
                result = carver.resizeHorizontalDP(targetHeight, visualize);
            }
        }
        
        return result;
    }
};

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <input_image> <target_width> <target_height> [method]" << std::endl;
        std::cout << "Method: 'dp' (default) or 'greedy'" << std::endl;
        return -1;
    }
    
    std::string inputPath = argv[1];
    int targetWidth = std::stoi(argv[2]);
    int targetHeight = std::stoi(argv[3]);
    std::string method = (argc > 4) ? argv[4] : "dp";
    
    // Load image
    cv::Mat image = imread(inputPath);
    if (image.empty()) {
        std::cout << "Error: Could not load image " << inputPath << std::endl;
        return -1;
    }
    
    std::cout << "Original size: " << image.cols << "x" << image.rows << std::endl;
    std::cout << "Target size: " << targetWidth << "x" << targetHeight << std::endl;
    
    // Validate target dimensions
    if (targetWidth > image.cols || targetHeight > image.rows) {
        cout << "Error: Target dimensions must be smaller than original" << std::endl;
        return -1;
    }
    
    // Display original
    imshow("Original Image", image);
    
    // Perform seam carving
    SeamCarver carver(image);
    cv::Mat result;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    if (method == "greedy") {
        std::cout << "\nUsing Greedy Algorithm..." << std::endl;
        result = carver.resize(targetWidth, targetHeight, false, false);
    } else {
        std::cout << "\nUsing Dynamic Programming..." << std::endl;
        result = carver.resize(targetWidth, targetHeight, true, false);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\nProcessing completed in " << duration.count() << " ms" << std::endl;
    std::cout << "Result size: " << result.cols << "x" << result.rows << std::endl;
    
    // Display result
    imshow("Seam Carved Image", result);
    
    // Save result
    std::string outputPath = "output_" + method + ".jpg";
    imwrite(outputPath, result);
    std::cout << "Result saved to: " << outputPath << std::endl;
    
    // Compare with simple scaling
    cv::Mat scaled;
    resize(image, scaled, Size(targetWidth, targetHeight));
    imshow("Simple Scaling (for comparison)", scaled);
    imwrite("output_scaled.jpg", scaled);
    
    waitKey(0);
    
    return 0;
}