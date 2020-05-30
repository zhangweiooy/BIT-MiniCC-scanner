else if (c == '[')
            {
                tokenStream += generatorToken(tokenNum, "[", "'['");
                tokenNum++;
                state = INITIAL;
            }
            else if (c == ']')
            {
                tokenStream += generatorToken(tokenNum, "]", "']'");
                tokenNum++;
                state = INITIAL;
            }