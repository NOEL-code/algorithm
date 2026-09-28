package dev.study.member;

import java.nio.charset.StandardCharsets;
import java.security.*;
import java.util.*;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;

final class Passwords {
    private static final SecureRandom RANDOM = new SecureRandom();
    private static final int ITERATIONS = 600_000;
    static String token() { byte[] bytes = new byte[32]; RANDOM.nextBytes(bytes); return Base64.getUrlEncoder().withoutPadding().encodeToString(bytes); }
    static String hash(String password) {
        byte[] salt = new byte[16]; RANDOM.nextBytes(salt);
        return Base64.getEncoder().encodeToString(salt) + ":" + Base64.getEncoder().encodeToString(derive(password, salt));
    }
    static boolean matches(String password, String stored) {
        String[] parts = stored.split(":");
        return MessageDigest.isEqual(Base64.getDecoder().decode(parts[1]), derive(password, Base64.getDecoder().decode(parts[0])));
    }
    private static byte[] derive(String password, byte[] salt) {
        var spec = new PBEKeySpec(password.toCharArray(), salt, ITERATIONS, 256);
        try { return SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256").generateSecret(spec).getEncoded(); }
        catch (GeneralSecurityException e) { throw new IllegalStateException(e); }
        finally { spec.clearPassword(); }
    }
    static String digest(String token) {
        try { return HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(token.getBytes(StandardCharsets.UTF_8))); }
        catch (GeneralSecurityException e) { throw new IllegalStateException(e); }
    }
}
