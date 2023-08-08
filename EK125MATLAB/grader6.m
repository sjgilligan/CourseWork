bankingsystem('b',1000)
function balance = bankingsystem(mode, amount)
    persistent curr_balance
    curr_balance = 500;
    switch mode
        case 'd'
            curr_balance = curr_balance + amount;
            balance = curr_balance;
        case 'w'
            if curr_balance < amount
                balance = curr_balance;
            else
                curr_balance = curr_balance - amount;
                balance = curr_balance;
            end
        case 'b'
            balance = curr_balance;
    end
end