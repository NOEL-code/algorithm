/**
 * 학습: 비밀번호와 세션 토큰의 위협 모델 차이 사람의 비밀번호에는 salt와 느린 KDF, 충분히 무작위인 세션 토큰에는 빠른 SHA-256을 사용한다. PBKDF2는 JDK
 * 표준 구현에 위임한다. 비밀번호를 암호화해 복호화하거나 직접 암호 알고리즘을 만들지 않는다. 현재 저장 형식에는 반복 횟수가 없다. ITERATIONS를 바꾸기 전에
 * 버전/비용 메타데이터와 점진 재해시가 필요하다.
 */
package dev.study.member;

import java.nio.charset.StandardCharsets;
import java.security.*;
import java.util.*;

import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

final class Passwords {
    private static final SecureRandom RANDOM = new SecureRandom();
    private static final int ITERATIONS = 600_000;

    // UUID가 아닌 256비트 난수로 인증 비밀을 만든다. Base64는 인코딩이며 암호화가 아니다.
    static String token() {
        byte[] bytes = new byte[32];
        RANDOM.nextBytes(bytes);
        return Base64.getUrlEncoder().withoutPadding().encodeToString(bytes);
    }

    // salt는 비밀이 아니고 비밀번호마다 다르다. 같은 비밀번호도 저장 값이 달라 사전 계산을 어렵게 한다.
    static String hash(String password) {
        byte[] salt = new byte[16];
        RANDOM.nextBytes(salt);
        return Base64.getEncoder().encodeToString(salt)
                + ":"
                + Base64.getEncoder().encodeToString(derive(password, salt));
    }

    // 복호화 대신 후보 비밀번호를 다시 파생해 비교한다. MessageDigest.isEqual로 단순 문자열 비교를 피한다.
    static boolean matches(String password, String stored) {
        String[] parts = stored.split(":");
        return MessageDigest.isEqual(
                Base64.getDecoder().decode(parts[1]),
                derive(password, Base64.getDecoder().decode(parts[0])));
    }

    private static byte[] derive(String password, byte[] salt) {
        var spec = new PBEKeySpec(password.toCharArray(), salt, ITERATIONS, 256);
        try {
            return SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256")
                    .generateSecret(spec)
                    .getEncoded();
        } catch (GeneralSecurityException e) {
            throw new IllegalStateException(e);
        } finally {
            spec.clearPassword();
        }
    }

    // 무작위 고엔트로피 토큰은 느린 비밀번호 KDF와 다른 대상이다. DB에는 조회 가능한 고정 길이 해시만 둔다.
    static String digest(String token) {
        try {
            return HexFormat.of()
                    .formatHex(
                            MessageDigest.getInstance("SHA-256")
                                    .digest(token.getBytes(StandardCharsets.UTF_8)));
        } catch (GeneralSecurityException e) {
            throw new IllegalStateException(e);
        }
    }
}
