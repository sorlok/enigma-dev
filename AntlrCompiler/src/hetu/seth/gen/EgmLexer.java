// Generated from Egm.g4 by ANTLR 4.5

 package hetu.seth.gen;
 
 import java.util.HashMap;
 import java.util.Deque;
 import java.util.ArrayDeque;

import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class EgmLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.5", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, IntegerLiteral=14, FloatingPointLiteral=15, 
		StringLiteral=16, Identifier=17, WS=18, COMMENT=19, LINE_COMMENT=20;
	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	public static final String[] ruleNames = {
		"T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
		"T__9", "T__10", "T__11", "T__12", "IntegerLiteral", "FloatingPointLiteral", 
		"StringLiteral", "Identifier", "WS", "COMMENT", "LINE_COMMENT"
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


	  Deque<String> obj_scope = new ArrayDeque<>();    //Which object (e.g., "self") are we dealing with?


	public EgmLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Egm.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u0430\ud6d1\u8206\uad2d\u4417\uaef1\u8d80\uaadd\2\26\u0087\b\1\4\2"+
		"\t\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4"+
		"\13\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22"+
		"\t\22\4\23\t\23\4\24\t\24\4\25\t\25\3\2\3\2\3\2\3\2\3\2\3\3\3\3\3\4\3"+
		"\4\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3\n\3\n\3\13\3\13\3\f\3\f\3"+
		"\r\3\r\3\16\3\16\3\17\5\17J\n\17\3\17\6\17M\n\17\r\17\16\17N\3\20\5\20"+
		"R\n\20\3\20\6\20U\n\20\r\20\16\20V\3\21\3\21\6\21[\n\21\r\21\16\21\\\3"+
		"\21\3\21\3\22\3\22\7\22c\n\22\f\22\16\22f\13\22\3\23\6\23i\n\23\r\23\16"+
		"\23j\3\23\3\23\3\24\3\24\3\24\3\24\7\24s\n\24\f\24\16\24v\13\24\3\24\3"+
		"\24\3\24\3\24\3\24\3\25\3\25\3\25\3\25\7\25\u0081\n\25\f\25\16\25\u0084"+
		"\13\25\3\25\3\25\3t\2\26\3\3\5\4\7\5\t\6\13\7\r\b\17\t\21\n\23\13\25\f"+
		"\27\r\31\16\33\17\35\20\37\21!\22#\23%\24\'\25)\26\3\2\t\3\2\62;\4\2\60"+
		"\60\62;\3\2$$\4\2C\\c|\6\2\62;C\\aac|\5\2\13\f\16\17\"\"\4\2\f\f\17\17"+
		"\u008f\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2\2\2\2\13\3\2\2\2\2"+
		"\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3"+
		"\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3\2\2\2\2!\3\2\2\2"+
		"\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\3+\3\2\2\2\5\60\3\2\2\2"+
		"\7\62\3\2\2\2\t\64\3\2\2\2\13\66\3\2\2\2\r8\3\2\2\2\17:\3\2\2\2\21<\3"+
		"\2\2\2\23>\3\2\2\2\25@\3\2\2\2\27B\3\2\2\2\31D\3\2\2\2\33F\3\2\2\2\35"+
		"I\3\2\2\2\37Q\3\2\2\2!X\3\2\2\2#`\3\2\2\2%h\3\2\2\2\'n\3\2\2\2)|\3\2\2"+
		"\2+,\7y\2\2,-\7k\2\2-.\7v\2\2./\7j\2\2/\4\3\2\2\2\60\61\7*\2\2\61\6\3"+
		"\2\2\2\62\63\7+\2\2\63\b\3\2\2\2\64\65\7}\2\2\65\n\3\2\2\2\66\67\7\177"+
		"\2\2\67\f\3\2\2\289\7=\2\29\16\3\2\2\2:;\7-\2\2;\20\3\2\2\2<=\7/\2\2="+
		"\22\3\2\2\2>?\7,\2\2?\24\3\2\2\2@A\7\61\2\2A\26\3\2\2\2BC\7\60\2\2C\30"+
		"\3\2\2\2DE\7?\2\2E\32\3\2\2\2FG\7.\2\2G\34\3\2\2\2HJ\7/\2\2IH\3\2\2\2"+
		"IJ\3\2\2\2JL\3\2\2\2KM\t\2\2\2LK\3\2\2\2MN\3\2\2\2NL\3\2\2\2NO\3\2\2\2"+
		"O\36\3\2\2\2PR\7/\2\2QP\3\2\2\2QR\3\2\2\2RT\3\2\2\2SU\t\3\2\2TS\3\2\2"+
		"\2UV\3\2\2\2VT\3\2\2\2VW\3\2\2\2W \3\2\2\2XZ\7$\2\2Y[\n\4\2\2ZY\3\2\2"+
		"\2[\\\3\2\2\2\\Z\3\2\2\2\\]\3\2\2\2]^\3\2\2\2^_\7$\2\2_\"\3\2\2\2`d\t"+
		"\5\2\2ac\t\6\2\2ba\3\2\2\2cf\3\2\2\2db\3\2\2\2de\3\2\2\2e$\3\2\2\2fd\3"+
		"\2\2\2gi\t\7\2\2hg\3\2\2\2ij\3\2\2\2jh\3\2\2\2jk\3\2\2\2kl\3\2\2\2lm\b"+
		"\23\2\2m&\3\2\2\2no\7\61\2\2op\7,\2\2pt\3\2\2\2qs\13\2\2\2rq\3\2\2\2s"+
		"v\3\2\2\2tu\3\2\2\2tr\3\2\2\2uw\3\2\2\2vt\3\2\2\2wx\7,\2\2xy\7\61\2\2"+
		"yz\3\2\2\2z{\b\24\2\2{(\3\2\2\2|}\7\61\2\2}~\7\61\2\2~\u0082\3\2\2\2\177"+
		"\u0081\n\b\2\2\u0080\177\3\2\2\2\u0081\u0084\3\2\2\2\u0082\u0080\3\2\2"+
		"\2\u0082\u0083\3\2\2\2\u0083\u0085\3\2\2\2\u0084\u0082\3\2\2\2\u0085\u0086"+
		"\b\25\2\2\u0086*\3\2\2\2\f\2INQV\\djt\u0082\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}