#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace logic {

    template <typename Value, bool isConst>
    class virtual_iterator {
    private:

        typedef virtual_iterator self;

    public:

        typedef Value value_type;
        typedef virtual_iterator<value_type, true> base_const;
        typedef virtual_iterator<value_type, false> base_mut;
        typedef typename util::if_c<isConst, const value_type&, value_type&>::type access_type;

        virtual access_type operator* () = 0;
        virtual const value_type& operator* () const = 0;
        virtual self& operator++ () = 0;
        virtual bool operator!= (unsigned end) const = 0;
        virtual unsigned pos() const = 0;
        virtual void explicit_set(unsigned i) = 0;
        virtual void set(const base_const& other) = 0;
        virtual void set(const base_mut& other) = 0;
    };

    template <typename Matrix>
    class row_iterator : public virtual_iterator<typename Matrix::value_type, util::is_const<Matrix>::value> {
    public:

        typedef typename Matrix::value_type value_type;
        typedef Matrix matrix_type;

    protected:

        typedef virtual_iterator<value_type, util::is_const<Matrix>::value> base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
        typedef row_iterator self;
        typedef typename util::if_<util::is_const<Matrix>, const Matrix&, Matrix&>::type ref_type;
        typedef typename base::access_type access_type;

    public:

        access_type operator* ()
        {
            return val(row, current);
        }

        const value_type& operator* () const
        {
            return val(row, current);
        }

        base& operator++ ()
        {
            current++;
            return *this;
        }

        bool operator!= (unsigned end) const {
            return current < end;
        }

        unsigned pos() const
        {
            if (current < val.width()) {
                return current;
            }
            return val.width();
        }

        void explicit_set(unsigned i) { current = i; }

        void set(const base_const& other) { current = other.pos(); }
        void set(const base_mut& other) { current = other.pos(); }

        row_iterator(ref_type val, unsigned row) : val(val), row(row),
                                            current(0) {}
        row_iterator(const self& other) : val(other.val), row(other.row),
                                            current(other.current) {}
    private:

        ref_type val;
        unsigned row, current;
    };

    template <typename Matrix>
    class col_iterator : public virtual_iterator<typename Matrix::value_type, util::is_const<Matrix>::value> {
    public:

        typedef typename Matrix::value_type value_type;
        typedef Matrix matrix_type;

    protected:

        typedef virtual_iterator<value_type, util::is_const<Matrix>::value> base;
        typedef typename base::base_const base_const;
        typedef typename base::base_mut base_mut;
        typedef col_iterator self;
        typedef typename util::if_<util::is_const<Matrix>, const Matrix&, Matrix&>::type ref_type;
        typedef typename base::access_type access_type;

    public:

        access_type operator* ()
        {
            return val(current, col);
        }

        const value_type& operator* () const
        {
            return val(current, col);
        }

        base& operator++ ()
        {
            current++;
            return *this;
        }

        bool operator!= (unsigned end) const {
            return current < end;
        }

        unsigned pos() const
        {
            if (pos < val.height()) {
                return pos;
            }
            return val.height();
        }

        void explicit_set(unsigned i) { current = i; }

        void set(const base_const& other) { current = other.pos(); }
        void set(const base_mut& other) { current = other.pos(); }

        col_iterator(ref_type val, unsigned col) : val(val), col(col),
                                                current(0) {}
        col_iterator(const self& other) : val(other.val), col(other.col),
                                                current(other.current) {}
    private:

        ref_type val;
        unsigned col, current;
    };

}

#endif // ITERATOR_HPP
