/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#pragma once

#include <nori/common.h>

NORI_NAMESPACE_BEGIN


/**
 * \brief Discrete probability distribution
 * 
 * This data structure can be used to transform uniformly distributed
 * samples to a stored discrete probability distribution.
 * 
 * \ingroup libcore
 */
struct DiscretePDF {
public:
    /// Allocate memory for a distribution with the given number of entries
    explicit DiscretePDF(size_t nEntries = 0) {
        reserve(nEntries);
        clear();
    }

    /// Clear all entries
    void clear() {
        m_cdf.clear();
        m_cdf.push_back(0.0f);
        m_normalized = false;
    }

    /// Reserve memory for a certain number of entries
    void reserve(size_t nEntries) {
        m_cdf.reserve(nEntries+1);
        //两个reserve不一样(同名函数我rnm)，这个是vector::reserve，重新分配vector的容量
    }

    /// Append an entry with the specified discrete probability
    void append(float pdfValue) {
        m_cdf.push_back(m_cdf[m_cdf.size()-1] + pdfValue);//累积概率，pdfValue加上最后一个概率
    }

    /// Return the number of entries so far
    size_t size() const {
        return m_cdf.size()-1;
    }

    /// Access an entry by its index
    float operator[](size_t entry) const {//从cdf中返回pdf
        return m_cdf[entry+1] - m_cdf[entry];
    }

    /// Have the probability densities been normalized?
    bool isNormalized() const {
        return m_normalized;
    }

    /**
     * \brief Return the original (unnormalized) sum of all PDF entries
     *
     * This assumes that \ref normalize() has previously been called
     */
    float getSum() const {//原始（未归一化）概率和
        return m_sum;
    }

    /**
     * \brief Return the normalization factor (i.e. the inverse of \ref getSum())
     *
     * This assumes that \ref normalize() has previously been called
     */
    float getNormalization() const {//归一化因子，即原始概率和的倒数
        return m_normalization;
    }

    /**
     * \brief Normalize the distribution
     *
     * \return Sum of the (previously unnormalized) entries
     */
    float normalize() {//分布归一化
        m_sum = m_cdf[m_cdf.size()-1];//最后一项的累积概率
        if (m_sum > 0) {
            m_normalization = 1.0f / m_sum;//归一化因子
            for (size_t i=1; i<m_cdf.size(); ++i) 
                m_cdf[i] *= m_normalization;//就尼玛离谱，每一个乘一个因子
            m_cdf[m_cdf.size()-1] = 1.0f;
            m_normalized = true;
        } else {
            m_normalization = 0.0f;//归一化因子
        }
        return m_sum;
    }

    //给一个[0,1]随机概率，返回cdf中的离散区间，用的是inversion method的原理，下面三个都是
    /**
     * \brief %Transform a uniformly distributed sample to the stored distribution
     * 
     * \param[in] sampleValue
     *     An uniformly distributed sample on [0,1]
     * \return
     *     The discrete index associated with the sample
     */
    size_t sample(float sampleValue) const {
        std::vector<float>::const_iterator entry = 
                std::lower_bound(m_cdf.begin(), m_cdf.end(), sampleValue);
        size_t index = (size_t) std::max((ptrdiff_t) 0, entry - m_cdf.begin() - 1);
        return std::min(index, m_cdf.size()-2);
    }

    /**
     * \brief %Transform a uniformly distributed sample to the stored distribution
     * 
     * \param[in] sampleValue
     *     An uniformly distributed sample on [0,1]
     * \param[out] pdf
     *     Probability value of the sample
     * \return
     *     The discrete index associated with the sample
     */
    size_t sample(float sampleValue, float &pdf) const {
        size_t index = sample(sampleValue);
        pdf = operator[](index);
        return index;
    }

    /**
     * \brief %Transform a uniformly distributed sample to the stored distribution
     * 
     * The original sample is value adjusted so that it can be "reused".
     *
     * \param[in, out] sampleValue
     *     An uniformly distributed sample on [0,1]
     * \return
     *     The discrete index associated with the sample
     */
    size_t sampleReuse(float &sampleValue) const {
        size_t index = sample(sampleValue);
        sampleValue = (sampleValue - m_cdf[index])
            / (m_cdf[index + 1] - m_cdf[index]);
        return index;
    }

    /**
     * \brief %Transform a uniformly distributed sample. 
     * 
     * The original sample is value adjusted so that it can be "reused".
     *
     * \param[in,out]
     *     An uniformly distributed sample on [0,1]
     * \param[out] pdf
     *     Probability value of the sample
     * \return
     *     The discrete index associated with the sample
     */
    size_t sampleReuse(float &sampleValue, float &pdf) const {
        size_t index = sample(sampleValue, pdf);
        sampleValue = (sampleValue - m_cdf[index])
            / (m_cdf[index + 1] - m_cdf[index]);
        return index;
    }

    /**
     * \brief Turn the underlying distribution into a
     * human-readable string format
     */
    std::string toString() const {
        std::string result = tfm::format("DiscretePDF[sum=%f, "
            "normalized=%f, pdf = {", m_sum, m_normalized);

        for (size_t i=0; i<m_cdf.size(); ++i) {
            result += std::to_string(operator[](i));
            if (i != m_cdf.size()-1)
                result += ", ";
        }
        return result + "}]";
    }
private:
    std::vector<float> m_cdf;
    float m_sum, m_normalization;//未归一化概率和，归一化因子，总之就是倒数关系，为什么要有两个参数呢？
    bool m_normalized;
};

NORI_NAMESPACE_END
