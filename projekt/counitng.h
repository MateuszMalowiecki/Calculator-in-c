bool isop(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool isnum(char c) {
	return isdigit(c);
}

bool isbracket(char c) {
	return c == '(' || c == ')';
}

bool isinput(char c) {
	return isop(c) || isnum(c) || isbracket(c) || c=='.';
}

bool iserease(char c) {
	return c == 127 || c == 8;
}

int kw(char c) {
    if (c=='/') return 2;
    if (c=='%') return 2;
    if (c=='*') return 2;
    if (c=='-') return 1;
    if (c=='+') return 1;
    return 0;
}

char* convertToONP(char* expr) {
	// i - expression index, k - output index, l - heap top index
    int i=0,k=0,l=0;

    char* expr_zero = malloc(sizeof(char) * (strlen(expr) + 2));
    strcpy(expr_zero, "0");
    strcat(expr_zero, expr);

    char* output = (char*)malloc(CALC_MAX_INPUT * sizeof(char));
    char heap[CALC_MAX_INPUT];

    while (expr_zero[i] != '\0') {
		if (isnum(expr_zero[i]) || expr_zero[i] == ',' || expr_zero[i] == '.') {
			output[k] = expr_zero[i];
			++k;
		}
		else if (isop(expr_zero[i])) {
			output[k] = ' '; ++k;
			while (l > 0 && (kw(expr_zero[i]) <= kw(heap[l-1]))) {
				output[k] = heap[l-1];
				++k, --l;
			}
			heap[l] = expr_zero[i];
			l++;
		}
		else if (expr_zero[i] == '(') {
			heap[l] = expr_zero[i];
			++l;
		}
		else if (expr_zero[i] == ')') {
			output[k] = ' '; ++k;
			while (heap[l-1] != '(') {
				output[k] = heap[l-1];
				++k, --l;
			}
			--l;
		}

		++i;
	}

    while (l>0) {
		output[k]=heap[l-1];
		++k, --l;
	}

	output[k]='\0';

	free(expr_zero);
    return output;
}

void evalONP(char* exprONP, mpf_t evaluation) {

    mpf_t stack[CALC_MAX_INPUT];
    unsigned int stack_idx = 0;

    mpf_t a, b, d, e, f;
    unsigned int i = 0;

    mpf_t n;
    bool mpf_read_flag = false;
    char buff[1000];
    char c;

    mpf_init(a); mpf_init(b); mpf_init(d); mpf_init(e); mpf_init(f);
    while((c = exprONP[i]) != '\0') {

        if (c == ' ') {
            ++i;
            continue;
        }

        int k = 0;
        while (isnum(c) || c == '.') {
            buff[k] = c;
            mpf_read_flag = true;

            ++i; ++k;
            c = exprONP[i];
        }
        if (mpf_read_flag) {
            mpf_read_flag = false;
            buff[k] = '\0';
            mpf_init_set_str(n, buff, 10);
            mpf_init_set(stack[stack_idx++], n);
            continue;
        }
        else {
            mpf_set(a, stack[--stack_idx]);
            mpf_set(b, stack[--stack_idx]);

            switch(c) {
            case '+':
                mpf_add(e, a, b); mpf_set(stack[stack_idx++], e); break;
            case '-':
                mpf_sub(e, b, a); mpf_set(stack[stack_idx++], e); break;
            case '*':
                mpf_mul(e, a, b); mpf_set(stack[stack_idx++], e); break;
            case '/':
                mpf_div(e, b, a); mpf_set(stack[stack_idx++], e); break;
            case '%':
                mpf_div(d, b, a);
                mpf_mul(f, d, a);
                mpf_sub(e, f, b);
                mpf_set(stack[stack_idx++], e); break;
            }
        }
        ++i;
    }
    mpf_set(evaluation, stack[0]);
}

char* convertToStrRep(mpf_t n) {
    mp_exp_t exp;

    char* chflt = (char*)malloc(sizeof(char) * CALC_MAX_INPUT + 3);
    char* mts = mpf_get_str(NULL, &exp, 10, CALC_MAX_INPUT, n);

	if (exp > 0) {

		if (strlen(mts) > 0 && mts[0] == '-') {
			++exp;
		}

		if (strlen(mts) <= (unsigned)exp) {
			strncpy(chflt, mts, strlen(mts));
			for(int i = strlen(mts); i < exp; ++i) {
				chflt[i] = '0';
			}
		} else {
			strncpy(chflt, mts, exp);
			chflt[exp] = '.';

			strncpy(chflt + sizeof(char)*(exp + 1), mts + sizeof(char)*(exp), strlen(mts) - exp);
		}

	} else if (exp <= 0) {
		chflt[0] = '0';

		if (strcmp(mts, "0") && exp == 0) {
			chflt[1] = '\0';
		} else {
			chflt[1] = '.';
			for(int i = 0; i < abs(exp); ++i) {
				chflt[i + 2] = '0';
			}
			strncpy(chflt + sizeof(char)*(abs(exp) + 2), mts, strlen(mts) - abs(exp));
		}
	}
    return chflt;
}
