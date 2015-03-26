(def (map f x)
    (if (eq? x '())
        '()
        (cons (f (car x)) (map f (cdr x)))
    )
)

(map print '(1 2 3))
(print "\n")
