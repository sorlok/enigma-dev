// Generated from Egm.g4 by ANTLR 4.5

 package hetu.seth.gen;
 
 import java.util.HashMap;
 import java.util.Deque;
 import java.util.ArrayDeque;

import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class EgmParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.5", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, IntegerLiteral=14, FloatingPointLiteral=15, 
		StringLiteral=16, Identifier=17, WS=18, COMMENT=19, LINE_COMMENT=20;
	public static final int
		RULE_rootRule = 0, RULE_block = 1, RULE_blockBody = 2, RULE_statement = 3, 
		RULE_expression = 4, RULE_paramList = 5, RULE_primary = 6, RULE_literal = 7;
	public static final String[] ruleNames = {
		"rootRule", "block", "blockBody", "statement", "expression", "paramList", 
		"primary", "literal"
	};

	private static final String[] _LITERAL_NAMES = {
		null, "'with'", "'('", "')'", "'{'", "'}'", "';'", "'+'", "'-'", "'*'", 
		"'/'", "'.'", "'='", "','"
	};
	private static final String[] _SYMBOLIC_NAMES = {
		null, null, null, null, null, null, null, null, null, null, null, null, 
		null, null, "IntegerLiteral", "FloatingPointLiteral", "StringLiteral", 
		"Identifier", "WS", "COMMENT", "LINE_COMMENT"
	};
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "Egm.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }


	  Deque<String> obj_scope = new ArrayDeque<>();    //Which object (e.g., "self") are we dealing with?

	public EgmParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}
	public static class RootRuleContext extends ParserRuleContext {
		public BlockBodyContext blockBody() {
			return getRuleContext(BlockBodyContext.class,0);
		}
		public TerminalNode EOF() { return getToken(EgmParser.EOF, 0); }
		public RootRuleContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_rootRule; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterRootRule(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitRootRule(this);
		}
	}

	public final RootRuleContext rootRule() throws RecognitionException {
		RootRuleContext _localctx = new RootRuleContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_rootRule);
		try {
			enterOuterAlt(_localctx, 1);
			{
			obj_scope.push("self");
			setState(17);
			blockBody();
			setState(18);
			match(EOF);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockContext extends ParserRuleContext {
		public Token Identifier;
		public BlockBodyContext blockBody() {
			return getRuleContext(BlockBodyContext.class,0);
		}
		public TerminalNode Identifier() { return getToken(EgmParser.Identifier, 0); }
		public BlockContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_block; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterBlock(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitBlock(this);
		}
	}

	public final BlockContext block() throws RecognitionException {
		BlockContext _localctx = new BlockContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_block);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(24);
			_la = _input.LA(1);
			if (_la==T__0) {
				{
				setState(20);
				match(T__0);
				setState(21);
				match(T__1);
				setState(22);
				((BlockContext)_localctx).Identifier = match(Identifier);
				setState(23);
				match(T__2);
				}
			}

			obj_scope.push(((BlockContext)_localctx).Identifier!=null?(((BlockContext)_localctx).Identifier!=null?((BlockContext)_localctx).Identifier.getText():null):obj_scope.peek());
			setState(27);
			match(T__3);
			setState(28);
			blockBody();
			setState(29);
			match(T__4);
			obj_scope.pop();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class BlockBodyContext extends ParserRuleContext {
		public HashMap<String, String> writeSymbols =  new HashMap<>();
		public HashMap<String, String> readSymbols =  new HashMap<>();
		public List<StatementContext> statement() {
			return getRuleContexts(StatementContext.class);
		}
		public StatementContext statement(int i) {
			return getRuleContext(StatementContext.class,i);
		}
		public BlockBodyContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_blockBody; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterBlockBody(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitBlockBody(this);
		}
	}

	public final BlockBodyContext blockBody() throws RecognitionException {
		BlockBodyContext _localctx = new BlockBodyContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_blockBody);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(35);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__0) | (1L << T__1) | (1L << T__3) | (1L << IntegerLiteral) | (1L << FloatingPointLiteral) | (1L << StringLiteral) | (1L << Identifier))) != 0)) {
				{
				{
				setState(32);
				statement();
				}
				}
				setState(37);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class StatementContext extends ParserRuleContext {
		public BlockContext block() {
			return getRuleContext(BlockContext.class,0);
		}
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public StatementContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_statement; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterStatement(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitStatement(this);
		}
	}

	public final StatementContext statement() throws RecognitionException {
		StatementContext _localctx = new StatementContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_statement);
		int _la;
		try {
			setState(43);
			switch (_input.LA(1)) {
			case T__0:
			case T__3:
				enterOuterAlt(_localctx, 1);
				{
				setState(38);
				block();
				}
				break;
			case T__1:
			case IntegerLiteral:
			case FloatingPointLiteral:
			case StringLiteral:
			case Identifier:
				enterOuterAlt(_localctx, 2);
				{
				setState(39);
				expression(0);
				{
				setState(41);
				_la = _input.LA(1);
				if (_la==T__5) {
					{
					setState(40);
					match(T__5);
					}
				}

				}
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class ExpressionContext extends ParserRuleContext {
		public ExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expression; }
	 
		public ExpressionContext() { }
		public void copyFrom(ExpressionContext ctx) {
			super.copyFrom(ctx);
		}
	}
	public static class AssignContext extends ExpressionContext {
		public Token access;
		public Token variable;
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public List<TerminalNode> Identifier() { return getTokens(EgmParser.Identifier); }
		public TerminalNode Identifier(int i) {
			return getToken(EgmParser.Identifier, i);
		}
		public AssignContext(ExpressionContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterAssign(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitAssign(this);
		}
	}
	public static class PrimContext extends ExpressionContext {
		public PrimaryContext primary() {
			return getRuleContext(PrimaryContext.class,0);
		}
		public PrimContext(ExpressionContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterPrim(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitPrim(this);
		}
	}
	public static class BinOpContext extends ExpressionContext {
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public BinOpContext(ExpressionContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterBinOp(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitBinOp(this);
		}
	}
	public static class FuncContext extends ExpressionContext {
		public TerminalNode Identifier() { return getToken(EgmParser.Identifier, 0); }
		public ParamListContext paramList() {
			return getRuleContext(ParamListContext.class,0);
		}
		public FuncContext(ExpressionContext ctx) { copyFrom(ctx); }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterFunc(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitFunc(this);
		}
	}

	public final ExpressionContext expression() throws RecognitionException {
		return expression(0);
	}

	private ExpressionContext expression(int _p) throws RecognitionException {
		ParserRuleContext _parentctx = _ctx;
		int _parentState = getState();
		ExpressionContext _localctx = new ExpressionContext(_ctx, _parentState);
		ExpressionContext _prevctx = _localctx;
		int _startState = 8;
		enterRecursionRule(_localctx, 8, RULE_expression, _p);
		int _la;
		try {
			int _alt;
			enterOuterAlt(_localctx, 1);
			{
			setState(61);
			switch ( getInterpreter().adaptivePredict(_input,6,_ctx) ) {
			case 1:
				{
				_localctx = new AssignContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;

				setState(48);
				switch ( getInterpreter().adaptivePredict(_input,4,_ctx) ) {
				case 1:
					{
					setState(46);
					((AssignContext)_localctx).access = match(Identifier);
					setState(47);
					match(T__10);
					}
					break;
				}
				setState(50);
				((AssignContext)_localctx).variable = match(Identifier);
				((BlockBodyContext)getInvokingContext(2)).writeSymbols.put((((AssignContext)_localctx).variable!=null?((AssignContext)_localctx).variable.getText():null), ((AssignContext)_localctx).access!=null?(((AssignContext)_localctx).access!=null?((AssignContext)_localctx).access.getText():null):obj_scope.peek());
				{
				setState(52);
				match(T__11);
				}
				setState(53);
				expression(2);
				}
				break;
			case 2:
				{
				_localctx = new PrimContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(54);
				primary();
				}
				break;
			case 3:
				{
				_localctx = new FuncContext(_localctx);
				_ctx = _localctx;
				_prevctx = _localctx;
				setState(55);
				match(Identifier);
				setState(56);
				match(T__1);
				setState(58);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__1) | (1L << IntegerLiteral) | (1L << FloatingPointLiteral) | (1L << StringLiteral) | (1L << Identifier))) != 0)) {
					{
					setState(57);
					paramList();
					}
				}

				setState(60);
				match(T__2);
				}
				break;
			}
			_ctx.stop = _input.LT(-1);
			setState(68);
			_errHandler.sync(this);
			_alt = getInterpreter().adaptivePredict(_input,7,_ctx);
			while ( _alt!=2 && _alt!=org.antlr.v4.runtime.atn.ATN.INVALID_ALT_NUMBER ) {
				if ( _alt==1 ) {
					if ( _parseListeners!=null ) triggerExitRuleEvent();
					_prevctx = _localctx;
					{
					{
					_localctx = new BinOpContext(new ExpressionContext(_parentctx, _parentState));
					pushNewRecursionContext(_localctx, _startState, RULE_expression);
					setState(63);
					if (!(precpred(_ctx, 3))) throw new FailedPredicateException(this, "precpred(_ctx, 3)");
					setState(64);
					_la = _input.LA(1);
					if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << T__6) | (1L << T__7) | (1L << T__8) | (1L << T__9))) != 0)) ) {
					_errHandler.recoverInline(this);
					} else {
						consume();
					}
					setState(65);
					expression(4);
					}
					} 
				}
				setState(70);
				_errHandler.sync(this);
				_alt = getInterpreter().adaptivePredict(_input,7,_ctx);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}

	public static class ParamListContext extends ParserRuleContext {
		public List<ExpressionContext> expression() {
			return getRuleContexts(ExpressionContext.class);
		}
		public ExpressionContext expression(int i) {
			return getRuleContext(ExpressionContext.class,i);
		}
		public ParamListContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_paramList; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterParamList(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitParamList(this);
		}
	}

	public final ParamListContext paramList() throws RecognitionException {
		ParamListContext _localctx = new ParamListContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_paramList);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(71);
			expression(0);
			setState(76);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__12) {
				{
				{
				setState(72);
				match(T__12);
				setState(73);
				expression(0);
				}
				}
				setState(78);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class PrimaryContext extends ParserRuleContext {
		public Token access;
		public Token variable;
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public LiteralContext literal() {
			return getRuleContext(LiteralContext.class,0);
		}
		public List<TerminalNode> Identifier() { return getTokens(EgmParser.Identifier); }
		public TerminalNode Identifier(int i) {
			return getToken(EgmParser.Identifier, i);
		}
		public PrimaryContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_primary; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterPrimary(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitPrimary(this);
		}
	}

	public final PrimaryContext primary() throws RecognitionException {
		PrimaryContext _localctx = new PrimaryContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_primary);
		try {
			setState(90);
			switch (_input.LA(1)) {
			case T__1:
				enterOuterAlt(_localctx, 1);
				{
				setState(79);
				match(T__1);
				setState(80);
				expression(0);
				setState(81);
				match(T__2);
				}
				break;
			case IntegerLiteral:
			case FloatingPointLiteral:
			case StringLiteral:
				enterOuterAlt(_localctx, 2);
				{
				setState(83);
				literal();
				}
				break;
			case Identifier:
				enterOuterAlt(_localctx, 3);
				{
				setState(86);
				switch ( getInterpreter().adaptivePredict(_input,9,_ctx) ) {
				case 1:
					{
					setState(84);
					((PrimaryContext)_localctx).access = match(Identifier);
					setState(85);
					match(T__10);
					}
					break;
				}
				setState(88);
				((PrimaryContext)_localctx).variable = match(Identifier);
				((BlockBodyContext)getInvokingContext(2)).readSymbols.put((((PrimaryContext)_localctx).variable!=null?((PrimaryContext)_localctx).variable.getText():null), ((PrimaryContext)_localctx).access!=null?(((PrimaryContext)_localctx).access!=null?((PrimaryContext)_localctx).access.getText():null):"unknown");
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static class LiteralContext extends ParserRuleContext {
		public TerminalNode IntegerLiteral() { return getToken(EgmParser.IntegerLiteral, 0); }
		public TerminalNode FloatingPointLiteral() { return getToken(EgmParser.FloatingPointLiteral, 0); }
		public TerminalNode StringLiteral() { return getToken(EgmParser.StringLiteral, 0); }
		public LiteralContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_literal; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).enterLiteral(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof EgmListener ) ((EgmListener)listener).exitLiteral(this);
		}
	}

	public final LiteralContext literal() throws RecognitionException {
		LiteralContext _localctx = new LiteralContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_literal);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(92);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & ((1L << IntegerLiteral) | (1L << FloatingPointLiteral) | (1L << StringLiteral))) != 0)) ) {
			_errHandler.recoverInline(this);
			} else {
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		case 4:
			return expression_sempred((ExpressionContext)_localctx, predIndex);
		}
		return true;
	}
	private boolean expression_sempred(ExpressionContext _localctx, int predIndex) {
		switch (predIndex) {
		case 0:
			return precpred(_ctx, 3);
		}
		return true;
	}

	public static final String _serializedATN =
		"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\3\26a\4\2\t\2\4\3\t"+
		"\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\3\2\3\2\3\2\3\2\3\3"+
		"\3\3\3\3\3\3\5\3\33\n\3\3\3\3\3\3\3\3\3\3\3\3\3\3\4\7\4$\n\4\f\4\16\4"+
		"\'\13\4\3\5\3\5\3\5\5\5,\n\5\5\5.\n\5\3\6\3\6\3\6\5\6\63\n\6\3\6\3\6\3"+
		"\6\3\6\3\6\3\6\3\6\3\6\5\6=\n\6\3\6\5\6@\n\6\3\6\3\6\3\6\7\6E\n\6\f\6"+
		"\16\6H\13\6\3\7\3\7\3\7\7\7M\n\7\f\7\16\7P\13\7\3\b\3\b\3\b\3\b\3\b\3"+
		"\b\3\b\5\bY\n\b\3\b\3\b\5\b]\n\b\3\t\3\t\3\t\2\3\n\n\2\4\6\b\n\f\16\20"+
		"\2\4\3\2\t\f\3\2\20\22e\2\22\3\2\2\2\4\32\3\2\2\2\6%\3\2\2\2\b-\3\2\2"+
		"\2\n?\3\2\2\2\fI\3\2\2\2\16\\\3\2\2\2\20^\3\2\2\2\22\23\b\2\1\2\23\24"+
		"\5\6\4\2\24\25\7\2\2\3\25\3\3\2\2\2\26\27\7\3\2\2\27\30\7\4\2\2\30\31"+
		"\7\23\2\2\31\33\7\5\2\2\32\26\3\2\2\2\32\33\3\2\2\2\33\34\3\2\2\2\34\35"+
		"\b\3\1\2\35\36\7\6\2\2\36\37\5\6\4\2\37 \7\7\2\2 !\b\3\1\2!\5\3\2\2\2"+
		"\"$\5\b\5\2#\"\3\2\2\2$\'\3\2\2\2%#\3\2\2\2%&\3\2\2\2&\7\3\2\2\2\'%\3"+
		"\2\2\2(.\5\4\3\2)+\5\n\6\2*,\7\b\2\2+*\3\2\2\2+,\3\2\2\2,.\3\2\2\2-(\3"+
		"\2\2\2-)\3\2\2\2.\t\3\2\2\2/\62\b\6\1\2\60\61\7\23\2\2\61\63\7\r\2\2\62"+
		"\60\3\2\2\2\62\63\3\2\2\2\63\64\3\2\2\2\64\65\7\23\2\2\65\66\b\6\1\2\66"+
		"\67\7\16\2\2\67@\5\n\6\48@\5\16\b\29:\7\23\2\2:<\7\4\2\2;=\5\f\7\2<;\3"+
		"\2\2\2<=\3\2\2\2=>\3\2\2\2>@\7\5\2\2?/\3\2\2\2?8\3\2\2\2?9\3\2\2\2@F\3"+
		"\2\2\2AB\f\5\2\2BC\t\2\2\2CE\5\n\6\6DA\3\2\2\2EH\3\2\2\2FD\3\2\2\2FG\3"+
		"\2\2\2G\13\3\2\2\2HF\3\2\2\2IN\5\n\6\2JK\7\17\2\2KM\5\n\6\2LJ\3\2\2\2"+
		"MP\3\2\2\2NL\3\2\2\2NO\3\2\2\2O\r\3\2\2\2PN\3\2\2\2QR\7\4\2\2RS\5\n\6"+
		"\2ST\7\5\2\2T]\3\2\2\2U]\5\20\t\2VW\7\23\2\2WY\7\r\2\2XV\3\2\2\2XY\3\2"+
		"\2\2YZ\3\2\2\2Z[\7\23\2\2[]\b\b\1\2\\Q\3\2\2\2\\U\3\2\2\2\\X\3\2\2\2]"+
		"\17\3\2\2\2^_\t\3\2\2_\21\3\2\2\2\r\32%+-\62<?FNX\\";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}