fun money_to_pence (pounds, shillings, pence) = 
    pounds * 20 * 12 + shillings * 12 + pence;

fun pence_to_money pences = 
    (pences div (20 * 12), pences mod (20 * 12) div 12, pences mod 12);

fun add_amounts (a, b) = pence_to_money(money_to_pence(a) + money_to_pence(b));
fun sub_amounts (a, b) = pence_to_money(money_to_pence(a) - money_to_pence(b));

add_amounts((1, 14, 9), (1, 9, 6)); (* (3, 4, 3) *)
sub_amounts((1, 14, 9), (1, 9, 6)); (* (0, 5, 3) *)
