#ifdef TEST_0006

#include "crashc.h"
#include "test_checker.h"

TESTS_START
REG_SUITE(1);
REG_SUITE(2);
TESTS_END

TESTSUITE(1) {
	clear_test_checker();
	int i=0;
    TESTCASE("testcase1", "") {
    	i += 1;
    	//ct_section_draw_tree(&rootSection, "issue14_%04d", i);
    	add_char('a');
        WHEN("when 1", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
        	add_char('b');
            int a = 6;
			int b = 0;
			add_char(' ');
			printf("a/b is %d\n", a/b);
			add_char('c');
        }
        add_char('d');
        THEN("then1", "") {
        	add_char('e');
        }
        add_char('f');
        WHEN("when2", "") {
        	add_char('g');
        }
        add_char('h');
        THEN("then2", "") {
        	add_char('i');
        }
        add_char('j');
        add_char('\0');
    }
    assert_and_reset_test_checker("ab ");
}

TESTSUITE(2) {
	clear_test_checker();
	int i=0;
    TESTCASE("testcase1", "") {
    	i += 1;
    	//ct_section_draw_tree(&rootSection, "issue14_%04d", i);
    	add_char('a');
        WHEN("when 1", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
        	add_char('b');
            int a = 6;
			int b = 0;
			printf("a/b is %d\n", a/b);
			add_char('c');
        }
    }

    add_char(' ');

    TESTCASE("testcase2", "") {
    	add_char('a');
		WHEN("when 1", "") {
			add_char('b');
		}
		add_char('c');
		WHEN("when 2", "") { //we enter here the first time. We then detect a signal and we mark that section are "signaled"
			add_char('d');
		}
		add_char('e');
    }
    assert_and_reset_test_checker("ab abceacde");
}

#endif
