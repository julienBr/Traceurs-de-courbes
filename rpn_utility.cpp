#include "rpn_utility.h"

std::map < QChar, std::pair<int, associativity>> RPN_utility::TAB_OP =
        {
            {'+', {2, LEFT}},
            {'-', {2, LEFT}},
            {'*', {3, LEFT}},
            {'/', {3, LEFT}},
            {'^', {4, RIGHT}}
       };


std::map< QChar, float(*)(const float& a, const float& b) > RPN_utility::OPERATION = {
    {'+', [](const float& a, const float& b ){return a+b;}},
    {'-', [](const float& a, const float& b ){return a-b;}},
    {'/', [](const float& a, const float& b ){return a/b;}},
    {'*', [](const float& a, const float& b ){return a*b;}},
    {'^', [](const float& a, const float& b ){return float(pow(a,b));}}
    };


std::map< QString, float(*)(const float& a) > RPN_utility::COMMON_FUNCTION = {
    {"cos",  [](const float& a ){return (float) cos(a);}},
    {"sin",  [](const float& a ){return (float) sin(a);}},
    {"sqrt", [](const float& a ){return (float) sqrt(a);}},
    {"log",  [](const float& a ){return (float) log10(a);}},
    {"ln",   [](const float& a ){return (float) log(a);}},
    {"exp",   [](const float& a ){return (float) exp(a);}},
    {"neg",   [](const float& a ){return (float) -a;}}
};


bool RPN_utility::isoperator(const QString &c)
{
    return c == "*" || c == "/" || c == "+" || c == "-" || c == "^" ;

}

bool RPN_utility::iscommonfunction(const QString & str)
{
    return COMMON_FUNCTION.find(str) != COMMON_FUNCTION.end();
}

bool RPN_utility::isnumber(const QString &s)
{
    if (s.isEmpty() || (s[0] != '-' && s[0] != '+' && !s[0].isNumber() ))
        return false;

    if (s.length() == 1 && ( s[0] == '-' || s[0] == '+' ) )
        return false;

    for ( auto i : s )
    {
        if (i != '-' && i != '.' && !i.isNumber() ) {
            return false;
        }

    }

    return true;

}



/* **** ***** ***** ***** ***** ***** ***** ***** */


RPN_utility::RPN_utility(const QString & raw, const QString & rpn) :
    raw_formula(raw), rpn_formula( rpn.split(QChar(' '), QString::SkipEmptyParts ) )
{
    if (rpn_formula.empty())
        rpn_formula = parse(raw).split(QChar(' '), QString::SkipEmptyParts);

}

QString RPN_utility::parse(QString formula)
{
       first_parser(formula);
       second_parser(formula);
       third_parser(formula);

       QString RPN_formula = main_parser(formula);


       return RPN_formula;
}




/* Parser permettant l'ajout des * entre
 *  - Constante * variable
 *  - Constante/variable * (...)
 */
void RPN_utility::first_parser(QString& form)
{

    //On supprime les blancs
    form.toLower();
    form.trimmed();

    QString copy(form);


    unsigned modif = 0;

    for(unsigned i = 1; i < (unsigned) form.length(); i++) {

        //On détecte si c'est un caractère
        if ( form[i].isLetter() ) {
            //On check si le caractère précédant est un chiffre et on ajoute une étoile
            if(form[i-1].isNumber() ) {
                    copy.insert(i + modif, "*");
                    modif++;
            }
       //On detecte si c'est une parenthèse ouvrante
         } else if (form[i] == '(' ) {
            //On check si le carartère précedant est un chiffre
            if( form[i-1].isNumber() ) {
                copy.insert(i + modif, "*");
                modif++;
            }
        }
    }


    form = copy;

}

/* Parser ajoutant les espaces entre les operateurs/tokens
 *  - 2*x --> 2 * x
 *
 */
void RPN_utility::second_parser(QString &form)
{

    QString copy(form);
    unsigned modif = 0;

    for (unsigned i = 0; i < (unsigned) form.length(); i++) {

        if (isoperator(QString(form[i])) || form[i] == '(' || form[i] == ')' || form[i] == ',')
        {
            copy.insert(i+modif, " " );
            modif ++;
            copy.insert(i+modif+1, " ");
            modif ++;
        }

    }

    form = copy;
}

/* Parser permettant de voir si c'est une negation ou un moins */
void RPN_utility::third_parser(QString &form )
{

    QStringList iss = form.split(QChar(' '), QString::SkipEmptyParts);
    QTextStream output(&form);
    form.clear();

    for (unsigned i = 0; i  < (unsigned) iss.length(); i++ )
    {

        if(i ==  0 && iss[i] == "-"  && iss[i+1].length() == 1 ) {
            output << " neg ( " << iss[i+1] << " ) ";
            i++;
        }  else if (i == 0 && iss[i] == "-" && iss[i+1].length() > 0 ) {

            output << "neg ( " << iss[i+1] << " ";
            unsigned o = i+2;
            unsigned cpt_para = 0;
            bool ok = false;
            while ( !ok ) {
                if (iss[o] == "(")
                    cpt_para++;
                else if (iss[o] == ")")
                    cpt_para--;

                output << iss[o] << " ";

                o++;
                ok = cpt_para == 0;
            }

            i = o;
            output << " ) ";

        } else if (i > 0 && iss[i] == "-" && iss[i-1] == "(" ) {
                output << " neg ( " << iss[i+1] << " ) ";
                i++;
        }else if (iss[i] == "-" && isoperator(iss[i-1]) ) {


            if ( iss[i+1].length() == 1 ) {
                    output << "neg ( " << iss[i+1] << " ) ";
                    i++;

            } else {
                output << "neg ( " << iss[i+1] << " ";
                unsigned o = i+2;
                unsigned cpt_para = 0;
                bool ok = false;
                while ( !ok ) {
                    if (iss[o] == "(")
                        cpt_para++;
                    else if (iss[o] == ")")
                        cpt_para--;

                    output << iss[o] << " ";

                    o++;
                    ok = cpt_para == 0;
                }

                i = o;
                output << " ) ";

                }
;
        } else {
            output << " " << iss[i] << " ";
        }
    }


    output.flush();

}


QString RPN_utility::main_parser(QString &formula) {


    QString output_string;
    QTextStream iss(&formula);

    QTextStream output(&output_string);
    QStack <QString> s;


    QString token;

    while(!iss.atEnd())
    {
        iss >> token;

        //If token is a number
        if( isnumber(token) || ( token.length() == 1 &&  token[0].isLetter() ) ) {
            output << token << " ";

        //if function separator
        } else if (token[0] ==  ',') {

            while (s.top() != "(" ) {
                output << s.top() << " ";
                s.pop();
            }

        //If token is an operator
        } else if( token.length() == 1 && isoperator(token) ) {

                       while ( !s.isEmpty() && ( (TAB_OP[token[0]].second == LEFT && TAB_OP[token[0]].first <= TAB_OP[s.top()[0]].first)
                                        ||
                                    (TAB_OP[token[0]].second == RIGHT && TAB_OP[token[0]].first < TAB_OP[s.top()[0]].first)
                                  )
                  ) {
                output << s.top() << " ";
                s.pop();
            }
            s.push(token);

       //Si c'est une fonction
       } else if (token.length() > 1 ) {
        s.push(token);


       //Si c'est une parenthèse ouvrante
       } else if (token == "(" ) {
            s.push(token);

       //Si c'est une parenthèse fermante
       } else if (token == ")" ) {

            while (s.top() != "(" ) {
                output << s.top() << " ";
                s.pop();
            }
            s.pop();

            if ( !s.empty() && s.top().length() > 1 ) {
                output << s.top() << " ";
                s.pop();
            }


        }

        token = "";



    }

    while (!s.empty()) {
        output << s.top() << " ";
        s.pop();
    }

    output.flush();
    return output_string;

}




float RPN_utility::calc(float x)
{
    std::stack<float> stack;
    //std::stringstream ss(rpn_formula);

    for (auto token : rpn_formula)
    {

        if ( isnumber(token) )
            stack.push(std::stof(token.toStdString()));
        else if ( token.length() == 1 && std::isalpha( token[0].toLatin1() ) )
            stack.push( x );

        else if (token.length() > 1) {

        float a = stack.top();
        stack.pop();



        stack.push ( COMMON_FUNCTION[token](a) ) ;

     } else if ( isoperator(token ) ) {
            float b = stack.top();
            stack.pop();
            float a = stack.top();
            stack.pop();

            stack.push ( OPERATION[ token[0].toLatin1() ](a,b) );
        }

    }

    return stack.top();
}

void RPN_utility::setFormula(const QString &new_form)
{
    rpn_formula = parse(new_form).split(QChar(' '), QString::SkipEmptyParts);
}


