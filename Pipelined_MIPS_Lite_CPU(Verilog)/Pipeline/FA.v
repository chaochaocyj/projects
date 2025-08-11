module FA(a, b, cin, cout, sum);

    input a, b, cin;
    output cout, sum;
    
    wire   e1, e2, e3, e4;


    xor	(e1, a, b); 
    xor	(sum, e1, cin);
    or	(e2, a, b);
    and	(e3, e2, cin);
    and	(e4, a ,b);
    or	(cout, e3, e4);	

endmodule