
class SuffixArr
{
public:
    /*
    * @brief sort the suffix array
    * @note algorithm will append 0 at the end, as the smalletst element,
    * @note please ensure the element in the data are positive
    * @note index of SuffixArr is [0,n]
    * @param arr, the datas
    * @param length of data
    */
    SuffixArr(const int* arr, const int n)
    {
        m_data.assign(arr, arr + n + 1);
        m_data[n] = 0;
        m_sa.resize(n + 1);
        m_rank.resize(n + 1);
        m_buckets.resize(n + 1);
        m_rankTmp.resize(n + 1);
        m_h.resize(n + 1, 0);
        m_height.resize(n + 1);

        Sort(n);


        m_h[0] = 0;
        for (int i = 0; i <= n; i++)
        {
            if (i > 0)
            {
                m_h[i] = std::max(0, m_h[i - 1] - 1);
            }

            if (m_rank[i] > 0)
            {
                const int j = m_sa[m_rank[i] - 1];
                while (m_data[i + m_h[i]] == m_data[j + m_h[i]])++m_h[i];
            }

            m_height[m_rank[i]] = m_h[i];
        }

#if 0
        Debug();
#endif
    }

    void Debug()
    {
        const int n = m_data.size() - 1;
        puts("data:");
        for (int i = 0; i <= n; i++)
        {
            cout << m_data[i] << " ";
        }
        cout << endl;

        cout << "SA" << endl;
        for (int i = 0; i <= n; i++)
        {
            for (int j = m_sa[i]; j <= n; ++j)
            {
                cout << m_data[j] << " ";
            }

            cout << "sa[" << i << "] = " << m_sa[i];
            cout << ", height[" << i << "] = " << m_height[i] << endl;
        }

        cout << "rank" << endl;

        for (int i = 0; i <= n; ++i)
        {
            for (int j = i; j <= n; ++j)
            {
                cout << m_data[j] << " ";
            }

            cout << "rank[" << i << "] = " << m_rank[i];
            cout << ",h[" << i << "] = " << m_h[i] << endl;
        }

    }

    int Height(int id) const
    {
        return m_height[id];
    }

    int SA(int rank) const
    {
        return m_sa[rank];
    }

    int Rank(int id) const
    {
        return m_rank[id];
    }

    int Size() const
    {
        return m_data.size() - 1;
    }

private:
    VI m_data;
    VI m_sa;//m_sa[0] = n is the useless letter
    VI m_rank;//m_rank[n]= 0, the n-th suffix is always smallest
    VI m_rankTmp;
    VVI m_buckets;
    VI m_h;
    VI m_height;

    void RadixSort(const int n, const int len)
    {
        //sort second key
        for (int b = 0; b <= n; ++b)m_buckets[b].clear();

        for (int i = 0; i <= n; ++i)
        {
            const int pos = m_sa[i] + len;
            if (pos > n)
            {
                m_buckets[0].push_back(m_sa[i]);
            }
            else
            {
                m_buckets[m_rank[pos]].push_back(m_sa[i]);
            }
        }

        m_sa.clear();

        FOR(b, 0, n)
        {
            for (int x : m_buckets[b])
            {
                m_sa.push_back(x);
            }
        }
    }

    void  Debug(int len, const std::vector<int>& x, const int n, const char* arrName)
    {
        printf("%s info, len = %d\n", arrName, len);
        for (int i = 0; i <= n; ++i)
        {
            printf("%s[%d]=%d\n", arrName, i, x[i]);
        }
    }

    int Compare(int i, int j, int len, int n)
    {
        auto r = [&](const int pos)->int
            {
                if (pos > n)return 0;
                return m_rank[pos];
            };
        if (m_rank[i] != m_rank[j])
        {
            return m_rank[i] - m_rank[j];
        }

        return r(i + len) - r(j + len);
    }

    /*
    * @brief:valid index 0-n, data[n] is the smallest element,that not belong to
    */
    void Sort(const int n)
    {
        //sort length of 1, collect

        std::vector<int> desc(m_data.begin(), m_data.end());
        sort(desc.begin(), desc.end());
        desc.erase(unique(desc.begin(), desc.end()), desc.end());

        //i-th suffix rank
        for (int i = 0; i <= n; ++i)
            m_rank[i] = lower_bound(desc.begin(), desc.end(), m_data[i]) - desc.begin();

        for (int i = 0; i <= n; ++i)m_sa[i] = i;

        //Debug(1, m_sa, n, "sa");
        //Debug(1, m_rank, n, "rank");

        for (int len = 1; len < n; len <<= 1)
        {
            RadixSort(n, len);

            RadixSort(n, 0);

            for (int i = 0; i <= n; ++i)
            {
                int j = i;
                while (j <= n && Compare(m_sa[i], m_sa[j], len, n) == 0)
                {
                    j++;
                }

                for (int iter = i; iter < j; iter++)m_rankTmp[m_sa[iter]] = i;

                i = j - 1;
            }

            swap(m_rankTmp, m_rank);

            //Debug(len<<1, m_sa, n, "sa");
            //Debug(len<<1, m_rank, n, "rank");
        }
    }
};
